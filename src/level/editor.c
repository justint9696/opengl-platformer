/**
 * @file editor.c
 * @author Justin Tonkinson
 * @date 2024/06/10
 * @brief Level editor implementation functions.
 */

#include "level/editor.h"

#include "data/fsm.h"
#include "entity/table.h"
#include "game/input.h"
#include "graphics/drawing.h"
#include "graphics/ibo.h"
#include "graphics/renderer.h"
#include "graphics/vao.h"
#include "graphics/vbo.h"
#include "level/level.h"

#include <assert.h>
#include <cglm/struct.h>
#include <string.h>

static const uint32_t COLOR_RED_FADE = 0xFF000080;
static const uint32_t COLOR_BLUE_FADE = 0x0078D780;

/** @brief Level editor function types for the state manager. */
typedef void (*editor_fn_t)(editor_t *, world_t *);

static vec2s mouse_to_world(world_t *world) {
    return screen_to_world(world, mouse_position());
}

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

static void camera_enter(editor_t *self, world_t *world) {
    ivec2s mouse = mouse_position();
    self->start = (vec2s) { mouse.x, mouse.y };
}

static void camera_update(editor_t *self, world_t *world) {
    // if left mouse is not held, transition to idle state and return
    if (mouse_released(SDL_BUTTON_LEFT)) {
        fsm_transition(&self->fsm, ES_IDLE, editor_fn_t, self, world);
        return;
    }

    // update camera position based on mouse movement
    ivec2s mouse = mouse_position();
    self->end = (vec2s) { mouse.x, mouse.y };

    vec2s diff = glms_vec2_sub(self->end, self->start);
    world->camera.pos
        = glms_vec3_add(world->camera.pos, (vec3s) { diff.x, -diff.y, 0 });

    self->start = self->end;
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

    vec2s mouse = mouse_to_world(world);
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

static void place_render(editor_t *self, world_t *world) {
    vec2s dim = (vec2s) { 50.f, 50.f };
    vec2s mouse
        = glms_vec2_sub(mouse_to_world(world), glms_vec2_scale(dim, 0.5f));
    draw_quad(mouse, dim, COLOR_RED_FADE);
}

static void edit_update(editor_t *self, world_t *world) {
    // if left mouse is pressed && there is an entity under the mouse,
    // transition to move state
    bool left = mouse_pressed(SDL_BUTTON_LEFT);
    bool right = mouse_pressed(SDL_BUTTON_RIGHT);

    if (!left && !right)
        return;

    vec2s mouse = mouse_to_world(world);
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

    assert(self->entity);

    // move all the selected entities
    vec2s mouse = mouse_to_world(world);
    self->entity->body.pos = glms_vec2_sub(mouse, self->offset);
}

static void idle_update(editor_t *self, world_t *world) {
    // if left mouse is pressed, transition to camera state
    if (mouse_held(SDL_BUTTON_LEFT, 0)) {
        fsm_transition(&self->fsm, ES_CAMERA, editor_fn_t, self, world);
        return;
    }

    // if shift is pressed, transition to highlight state
    if (button_held(SDL_SCANCODE_LSHIFT, 0) && mouse_pressed(SDL_BUTTON_LEFT)) {
        fsm_transition(&self->fsm, ES_HIGHLIGHT, editor_fn_t, self, world);
        return;
    }

    // if 'a' is pressed, transition to place state
    if (button_pressed(SDL_SCANCODE_A)) {
        fsm_transition(&self->fsm, ES_PLACE, editor_fn_t, self, world);
        return;
    }

    // if ctrl and left mouse are pressed, transition to select state
    if (button_held(SDL_SCANCODE_LCTRL, 0)) {
        fsm_transition(&self->fsm, ES_SELECT, editor_fn_t, self, world);
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

static void select_update(editor_t *self, world_t *world) {
    if (button_released(SDL_SCANCODE_LCTRL)) {
        fsm_transition(&self->fsm, ES_EDIT, editor_fn_t, self, world);
        return;
    }

    if (mouse_pressed(SDL_BUTTON_LEFT)) {
        vec2s mouse = mouse_to_world(world);
        self->entity = get_hovered_entity(world, mouse);
        return;
    }
}

static void highlight_enter(editor_t *self, world_t *world) {
    self->select.dim = GLMS_VEC2_ZERO;
    self->select.pos = mouse_to_world(world);
}

static void highlight_exit(editor_t *self, world_t *world) {
    // TODO: add all higlighted entities to dynamic array
}

static void highlight_update(editor_t *self, world_t *world) {
    if (button_released(SDL_SCANCODE_LSHIFT)
        || mouse_released(SDL_BUTTON_LEFT)) {
        fsm_transition(&self->fsm, ES_IDLE, editor_fn_t, self, world);
        return;
    }

    self->select.dim = glms_vec2_sub(mouse_to_world(world), self->select.pos);
}

static void highlight_render(editor_t *self, world_t *world) {
    draw_quad(self->select.pos, self->select.dim, COLOR_BLUE_FADE);
}

static void render(editor_t *self, world_t *world) {
    if (self->entity) {
        draw_quad(self->entity->body.pos, self->entity->body.dim,
                  COLOR_RED_FADE);
    }
}

void editor_init(editor_t *self) {
    memset(self, 0, sizeof(editor_t));
    fsm_init(&self->fsm, ES_MAX, ES_IDLE);
    fsm_add(&self->fsm, &(state_t) { .update = idle_update, .render = render });
    fsm_add(&self->fsm, &(state_t) { .update = edit_update, .render = render });
    fsm_add(&self->fsm, &(state_t) {
        .init = move_enter, .destroy = move_exit,
        .render = render, .update = move_update,
    });
    fsm_add(&self->fsm, &(state_t) {
        .update = place_update,
        .render = place_render,
    });
    fsm_add(&self->fsm, &(state_t) {
        .update = select_update,
        .render = render,
    });
    fsm_add(&self->fsm, &(state_t) {
        .init = highlight_enter,
        .destroy = highlight_exit,
        .render = highlight_render,
        .update = highlight_update,
    });
    fsm_add(&self->fsm, &(state_t) {
        .init = camera_enter,
        .update = camera_update,
        .render = render,
    });

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
    if (button_held(SDL_SCANCODE_RCTRL, 0)
        && button_held(SDL_SCANCODE_LSHIFT, 0)
        && button_pressed(SDL_SCANCODE_E)) {
        level_export(world, "data/demo.dat");
    }
    fsm_update(&self->fsm, editor_fn_t, self, world);
}

static char *state_to_string(editor_t *self) {
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
        case ES_HIGHLIGHT:
            return "Highlight";
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

    fsm_render(&self->fsm, editor_fn_t, self, world);

    vao_unbind();
    vbo_unbind();
    ibo_unbind();
}
