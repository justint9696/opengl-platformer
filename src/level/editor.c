#include "level/editor.h"

#include "data/fsm.h"
#include "data/grid.h"
#include "entity/table.h"
#include "game/input.h"
#include "graphics/drawing.h"
#include "graphics/ibo.h"
#include "graphics/renderer.h"
#include "graphics/vao.h"
#include "graphics/vbo.h"
#include "world/world.h"

#include <assert.h>
#include <cglm/struct.h>
#include <string.h>

typedef void (*editor_fn_t)(editor_t *, world_t *);

static entity_t *get_hovered_entity(world_t *world, vec2s mouse) {
    collider_t arr[64];
    size_t n = world_get_colliders(world, &(entity_t) { .body.pos = mouse },
                                   arr, 64);

    for (size_t i = 0; i < n; i++) {
        collider_t *tmp = &arr[i];
        if (aabb_intersect_2d(&tmp->box, mouse)) {
            return tmp->data;
        }
    }

    return NULL;
}

static void camera_update(editor_t *self, world_t *world) {
    // if left mouse is not held, transition to idle state and return

    // update camera position based on mouse movement
}

static void place_update(editor_t *self, world_t *world) {
    // if 'esc' is pressed, transition to idle state
    if (button_pressed(SDL_SCANCODE_ESCAPE)) {
        fsm_transition(&self->fsm, ES_IDLE, editor_fn_t, self, world);
        return;
    }

    // if left mouse is not pressed
    if (!mouse_pressed(SDL_BUTTON_LEFT))
        return;

    vec2s mouse = screen_to_world(world, mouse_position());
    if (!world_is_on_screen(world, mouse))
        return;

    if (get_hovered_entity(world, mouse))
        return;

    // place entity at mouse world position and transition to edit mode
    create_fn_t fn = table_lookup(ET_PLATFORM);
    assert(fn);

    vec2s dim = (vec2s) { 50.f, 50.f };
    vec2s pos = glms_vec2_sub(mouse, glms_vec2_scale(dim, 0.5f));
    self->entity = fn(pos, dim, world);

    // calculate the offset from the entity pos to the mouse
    self->offset = glms_vec2_sub(mouse, self->entity->body.pos);

    fsm_transition(&self->fsm, ES_MOVE, editor_fn_t, self, world);
}

static void edit_update(editor_t *self, world_t *world) {
    // if left mouse is pressed && there is an entity under the mouse,
    // transition to move state
    bool left = mouse_pressed(SDL_BUTTON_LEFT);
    bool right = mouse_pressed(SDL_BUTTON_RIGHT);

    if (!left && !right)
        return;

    vec2s mouse = screen_to_world(world, mouse_position());
    if (self->entity != get_hovered_entity(world, mouse)) {
        self->entity = NULL;
        fsm_transition(&self->fsm, ES_IDLE, editor_fn_t, self, world);
        return;
    }

    // if right mouse pressed && there is an entity under the mouse,
    // delete the selected entities and transitoin to idle state
    if (left) {
        // calculate the offset from the entity pos to the mouse
        self->offset = glms_vec2_sub(mouse, self->entity->body.pos);

        fsm_transition(&self->fsm, ES_MOVE, editor_fn_t, self, world);
        return;
    }

    if (right) {
        entity_destroy(self->entity, world);
        self->entity = NULL;

        fsm_transition(&self->fsm, ES_IDLE, editor_fn_t, self, world);
        return;
    }
}

static void move_update(editor_t *self, world_t *world) {
    // if left mouse is not held, transition to previous state and return
    if (mouse_released(SDL_BUTTON_LEFT)) {
        int previous = self->fsm.previous;
        fsm_transition(&self->fsm, previous, editor_fn_t, self, world);
        return;
    }

    // move all the selected entities
    assert(self->entity);
    vec2s mouse = screen_to_world(world, mouse_position());
    self->entity->body.pos = glms_vec2_sub(mouse, self->offset);
}

static void idle_update(editor_t *self, world_t *world) {
    // if left mouse is pressed, transition to camera state

    // if shift is pressed, transition to select state

    // if 'a' is pressed, transition to place state
    if (button_pressed(SDL_SCANCODE_A)) {
        fsm_transition(&self->fsm, ES_PLACE, editor_fn_t, self, world);
        return;
    }

    // if ctrl and left mouse are pressed, check if there is an entity under
    // mouse
    if (button_held(SDL_SCANCODE_LCTRL, 0) && mouse_pressed(SDL_BUTTON_LEFT)) {
        vec2s mouse = screen_to_world(world, mouse_position());
        // if so, push it to array of selected entities and transition to edit
        // state
        self->entity = get_hovered_entity(world, mouse);
        return;
    }

    if (button_released(SDL_SCANCODE_LCTRL) && self->entity) {
        fsm_transition(&self->fsm, ES_EDIT, editor_fn_t, self, world);
        return;
    }
}

static void move_enter(editor_t *self, world_t *world) {
    assert(self->entity);

    if (self->entity->body.cell) {
        cell_remove(self->entity->body.cell, self);
    }
}

static void move_exit(editor_t *self, world_t *world) {
    assert(self->entity);

    vec2s pos = world_to_screen(world, self->entity->body.pos);
    cell_t *cell = grid_cell_from_pos(&world->grid, pos);
    assert(cell);

    cell_insert(cell, self->entity);
    self->entity->body.cell = cell;
}

static void select_update(editor_t *self, world_t *world) {}

void editor_init(editor_t *self) {
    memset(self, 0, sizeof(editor_t));
    fsm_init(&self->fsm, ES_MAX, ES_IDLE);
    fsm_add(&self->fsm, NULL, NULL, idle_update);
    fsm_add(&self->fsm, NULL, NULL, edit_update);
    fsm_add(&self->fsm, move_enter, move_exit, move_update);
    fsm_add(&self->fsm, NULL, NULL, place_update);
    fsm_add(&self->fsm, NULL, NULL, select_update);
    fsm_add(&self->fsm, NULL, NULL, camera_update);

    self->vao = vao_create();
    self->vbo = vbo_create(NULL, 0);
    self->ibo = vbo_create(NULL, 0);
}

void editor_destroy(editor_t *self) {
    fsm_destroy(&self->fsm);

    vao_destroy(&self->vao);
    vbo_destroy(&self->vbo);
    ibo_destroy(&self->ibo);
}

void editor_sync(editor_t *self, world_t *world) {
    fsm_update(&self->fsm, editor_fn_t, self, world);
}

static inline char *state_to_string(editor_t *self) {
    switch (self->fsm.current) {
        case ES_EDIT:
            return "Edit";
        case ES_MOVE:
            return "Move";
        case ES_PLACE:
            return "Place";
        case ES_SELECT:
            return "Select";
        case ES_CAMERA:
            return "Camera";
        default:
            return "Idle";
    }
}

void editor_render(editor_t *self, world_t *world) {
    draw_debug_text("Editor State: %s", state_to_string(self));

    renderer_use_shader(SHADER_DEFAULT);

    vao_bind(&self->vao);
    vbo_bind(&self->vbo);
    ibo_bind(&self->ibo);

    if (self->entity) {
        draw_quad(self->entity->body.pos, self->entity->body.dim, COLOR_RED);
    }

    vao_unbind();
    vbo_unbind();
    ibo_unbind();
}
