#include "entity/entity.h"

#include "data/array.h"
#include "graphics/drawing.h"
#include "world/world.h"

#define DECL_MODULE(_name)\
    extern void _name##_tick(entity_t *, world_t *, float);\
    extern void _name##_update(entity_t *, world_t *, float);

DECL_MODULE(input);
DECL_MODULE(collision);
DECL_MODULE(movement);
DECL_MODULE(physics);
DECL_MODULE(ai);

entity_t *entity_create(void *data, world_t *world) {
    uint32_t id = array_push((void **)&world->entities, data);
    entity_t *self = array_get(world->entities, id);

    self->id = id;
    if (self->init) {
        self->init(self, world);
    }

#ifdef DEBUG
    self->debug.lines = array_alloc(sizeof(vec2s), 32);
#endif

    if (self->body.solid) {
        grid_add(&world->grid, &self, world_to_screen(world, self->body.pos));
    }

    return self;
}

void entity_destroy(entity_t *self, world_t *world) {
#ifdef DEBUG
    array_free(self->debug.lines);
#endif

    if (self->destroy) {
        self->destroy(self, world);
    }
    array_remove(world->entities, self);
}

void entity_render(entity_t *self, world_t *world) {
#ifdef DEBUG
    // render debug lines
    size_t len = array_len(self->debug.lines);
    for (size_t i = 0; i < len; i += 2) {
        vec2s 
            from = self->debug.lines[i],
            to = self->debug.lines[i+1];
        draw_line(from, to, COLOR_RED);
    }
#endif

    if (self->render) {
        self->render(self, world);
    }
}

void entity_update(entity_t *self, world_t *world, float dt) {
    if (self->update) {
        self->update(self, world, dt);
    }
}

void entity_tick(entity_t *self, world_t *world, float dt) {
    if (self->tick) {
        self->tick(self, world, dt);
    }

    // should the entity respond to player input
    if (self->flags & F_PLAYER_CONTROLLED) {
        input_tick(self, world, dt);
    }

    // is entity controlled by an ai
    if (self->flags & F_AI_CONTROLLED) {
        ai_tick(self, world, dt);
    }

    // should the entity move and respond to physics
    if (self->flags & F_KINEMATIC) {
        physics_tick(self, world, dt);

        if (self->body.solid) { collision_tick(self, world, dt); }

        movement_tick(self, world, dt);
    }
}
