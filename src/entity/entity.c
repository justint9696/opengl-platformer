/**
 * @file entity.c
 * @author Justin Tonkinson
 * @date 2024/05/07
 * @brief Entity implementation functions.
 */

#include "entity/entity.h"

#include "data/array.h"
#include "world/world.h"
#include "util/log.h"

#include <assert.h>

#define _DECL_MODULE(_name)\
    extern void _name##_sync(entity_t *, world_t *, float);\
    extern void _name##_update(entity_t *, world_t *, float);

_DECL_MODULE(input);
_DECL_MODULE(collision);
_DECL_MODULE(movement);
_DECL_MODULE(physics);
_DECL_MODULE(ai);
_DECL_MODULE(camera_follow);

entity_t *entity_create(const void *data, world_t *world) {
    vec2s pos = ((entity_t *)data)->body.pos;
    page_t *page = chunk_page_from_pos(&world->chunk, pos);
    assert(page);

    int id;
    if ((id = array_push(page->entities, data)) == -1) {
        log_fatal("Could not create entity within page %d\n", page->index);
    }

    entity_t *self = &page->entities[id];
    self->id = id;

    entity_init(self, world);

    return self;
}

void entity_init(entity_t *self, world_t *world) {
    self->body.page = chunk_page_from_pos(&world->chunk, self->body.pos);
    /* assert(self->body.page); */

    if (self->init) {
        self->init(self, world);
    }

    if (self->body.solid && world_is_on_screen(world, self->body.pos)) {
        self->body.cell = grid_cell_from_pos(
            &world->grid, world_to_screen(world, self->body.pos));
        cell_insert(self->body.cell, self);
    }
}

void entity_destroy(entity_t *self, world_t *world) {
    if (self->destroy) {
        self->destroy(self, world);
    }

    if (self->body.solid && self->body.cell) {
        cell_remove(self->body.cell, self);
    }

    page_t *page = chunk_page_from_pos(&world->chunk, self->body.pos);
    assert(page);

    array_remove(page->entities, self);
}

void entity_render(entity_t *self, world_t *world) {
    if (self->render) {
        self->render(self, world);
    }
}

void entity_update(entity_t *self, world_t *world, float dt) {
    if (self->update) {
        self->update(self, world, dt);
    }
}

void entity_sync(entity_t *self, world_t *world, float dt) {
    if (self->sync) {
        self->sync(self, world, dt);
    }

    if (self->flags & EF_PLAYER_CONTROLLED) {
        input_sync(self, world, dt);
    }

    if (self->flags & EF_AI_CONTROLLED) {
        ai_sync(self, world, dt);
    }

    if (self->flags & EF_CAMERA_FOLLOW) {
        camera_follow_sync(self, world, dt);
    }

    bool visible = world_is_on_screen(world, self->body.pos);
    if (self->flags & EF_KINEMATIC) {
        if (visible) {
            physics_sync(self, world, dt);

            if (self->body.solid) {
                collision_sync(self, world, dt);
            }
        }
        movement_sync(self, world, dt);
    }

    if (!self->body.solid)
        return;

    if (!visible) {
        if (self->body.cell) {
            cell_remove(self->body.cell, self);
        }
        self->body.cell = NULL;
        return;
    }

    cell_t *cell = grid_cell_from_pos(&world->grid,
                                      world_to_screen(world, self->body.pos));
    if (cell == self->body.cell)
        return;

    if (self->body.cell) {
        cell_remove(self->body.cell, self);
    }

    cell_insert(cell, self);
    self->body.cell = cell;
}
