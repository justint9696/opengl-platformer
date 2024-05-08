#include "entity/entity.h"
#include "data/array.h"
#include "world/world.h"

#define DECL_MODULE(_name)\
    extern void _name##_tick(entity_s *, world_s *, float);\
    extern void _name##_update(entity_s *, world_s *, float);

DECL_MODULE(input);
DECL_MODULE(movement);

entity_s *entity_create(void *data, world_s *world) {
    uint32_t id = array_push((void **)&world->entities, data);
    entity_s *self = array_get(world->entities, id);

    self->id = id;
    if (self->init != NULL) {
        self->init(self, world);
    }

    return self;
}

void entity_destroy(entity_s *self, world_s *world) {
    if (self->destroy != NULL) {
        self->destroy(self, world);
    }
    array_remove(world->entities, self);
}

void entity_render(entity_s *self, world_s *world) {
    if (self->render != NULL) {
        self->render(self, world);
    }
}

void entity_update(entity_s *self, world_s *world, float dt) {
    if (self->update != NULL) {
        self->update(self, world, dt);
    }
}

void entity_tick(entity_s *self, world_s *world, float dt) {
    if (self->tick != NULL) {
        self->tick(self, world, dt);
    }

    if (self->flags & FLAG_PLAYER_INPUT) {
        input_tick(self, world, dt);
    }
    if (self->flags & FLAG_KINEMATIC) {
        movement_tick(self, world, dt);
    }
}
