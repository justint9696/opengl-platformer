#include "entity/entity.h"

#include "data/array.h"
#include "util/assert.h"
#include "world/world.h"

#define DECL_MODULE(_name)\
    extern void _name##_sync(entity_t *, world_t *, float);\
    extern void _name##_update(entity_t *, world_t *, float);

DECL_MODULE(input);
DECL_MODULE(collision);
DECL_MODULE(movement);
DECL_MODULE(physics);
DECL_MODULE(ai);
DECL_MODULE(camera_follow);

entity_t *entity_create(void *data, world_t *world) {
    vec2s pos = ((entity_t *)data)->body.pos;
    page_t *page = chunk_page_from_pos(&world->chunk, pos);
    int32_t id = array_push(page->entities, data);
    XASSERT(id != -1, "Could not create entity within page %u\n", page->index);

    entity_t *self = array_get(page->entities, id);

    self->id = id;
    self->body.page = page;

    if (self->init) {
        self->init(self, world);
    }

    if (self->body.solid && world_is_on_screen(world, self->body.pos)) {
        self->body.cell = grid_cell_from_pos(
            &world->grid, world_to_screen(world, self->body.pos));
        cell_insert(self->body.cell, self);
    }

    return self;
}

void entity_destroy(entity_t *self, world_t *world) {
    if (self->destroy) {
        self->destroy(self, world);
    }

    if (self->body.solid && self->body.cell) {
        cell_remove(self->body.cell, self);
    }

    page_t *page = chunk_page_from_pos(&world->chunk, self->body.pos);
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

    // should the entity respond to player input
    if (self->flags & F_PLAYER_CONTROLLED) {
        input_sync(self, world, dt);
    }

    // is entity controlled by an ai
    if (self->flags & F_AI_CONTROLLED) {
        ai_sync(self, world, dt);
    }

    // should the entity move and respond to physics
    if (self->flags & F_KINEMATIC) {
        physics_sync(self, world, dt);

        if (self->body.solid) {
            collision_sync(self, world, dt);
        }

        movement_sync(self, world, dt);
    }

    // should the camera follow the entity's movement
    if (self->flags & F_CAMERA_FOLLOW) {
        camera_follow_sync(self, world, dt);
    }

    if (!self->body.solid)
        return;

    if (!world_is_on_screen(world, self->body.pos)) {
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
