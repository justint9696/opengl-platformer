#include "level/editor.h"

#include "entity/table.h"
#include "game/input.h"
#include "util/aabb.h"
#include "util/log.h"

#include <assert.h>
#include <cglm/struct.h>

void editor_init(editor_t *self) {
    memset(self, 0, sizeof(editor_t));
}

static void get_hovered_entity(editor_t *self, world_t *world) {
    entity_t mouse = (entity_t) { 0 };
    mouse.body.pos = screen_to_world(world, mouse_get_position());

    if (!world_is_on_screen(world, mouse.body.pos))
        return;

    collider_t arr[64];
    size_t n = world_get_colliders(world, &mouse, arr, 64);

    for (size_t i = 0; i < n; i++) {
        collider_t *tmp = &arr[i];
        if (aabb_intersect_2d(&tmp->box, mouse.body.pos)) {
            self->entity = tmp->data;
            self->offset
                = glms_vec2_sub(mouse.body.pos, self->entity->body.pos);
            break;
        }
    }
}

void editor_sync(editor_t *self, world_t *world) {
    if (mouse_pressed(SDL_BUTTON_LEFT)) {
        // check if entity is under mouse
        get_hovered_entity(self, world);
        if (!self->entity) {
            place_entity(self, world);
        }
        
    } else if (mouse_pressed(SDL_BUTTON_RIGHT)) {
        // remove block
    }
}
