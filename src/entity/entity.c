#include "entity/entity.h"

#include "data/array.h"
#include "graphics/drawing.h"
#include "world/world.h"

#define _DECL_MODULE(_name)\
    extern void _name##_sync(entity_t *, world_t *, float);\
    extern void _name##_update(entity_t *, world_t *, float);

_DECL_MODULE(input);
_DECL_MODULE(collision);
_DECL_MODULE(movement);
_DECL_MODULE(physics);
_DECL_MODULE(ai);
_DECL_MODULE(camera_follow);

entity_t *entity_create(void *data, world_t *world) {
    page_t *page 
        = chunk_page_from_pos(&world->chunk, ((entity_t *)data)->body.pos);

    uint32_t id = array_push(page->entities, data);
    entity_t *self = array_get(page->entities, id);

    self->id = id;
    self->body.page = page;

    if (self->init) {
        self->init(self, world);
    }

#ifdef _DEBUG
    self->debug.lines = array_alloc(sizeof(vec2s), 32);
#endif // _DEBUG

    if (self->body.solid) {
        grid_add(&world->grid, &self, world_to_screen(world, self->body.pos));
    }

    return self;
}

void entity_destroy(entity_t *self, world_t *world) {
#ifdef _DEBUG
    array_free(self->debug.lines);
#endif // _DEBUG

    if (self->destroy) {
        self->destroy(self, world);
    }

    page_t *page = chunk_page_from_pos(&world->chunk, self->body.pos);
    array_remove(page->entities, self);
}

void entity_render(entity_t *self, world_t *world) {
#ifdef _DEBUG
    // render debug lines
    size_t len = array_len(self->debug.lines);
    for (size_t i = 0; i < len; i += 2) {
        vec2s from = self->debug.lines[i];
        vec2s to = self->debug.lines[i+1];
        draw_line(from, to, COLOR_RED);
    }
#endif // _DEBUG

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

        if (self->body.solid) { collision_sync(self, world, dt); }

        movement_sync(self, world, dt);
    }

    // should the camera follow the entity's movement
    if (self->flags & F_CAMERA_FOLLOW) {
        camera_follow_sync(self, world, dt);
    }
}
