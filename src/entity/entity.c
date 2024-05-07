#include "entity/entity.h"
#include "data/array.h"
#include "world/world.h"

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

void entity_update(entity_s *self, world_s *world) {
    if (self->update != NULL) {
        self->update(self, world);
    }
}

void entity_tick(entity_s *self, world_s *world) {
    if (self->tick != NULL) {
        self->tick(self, world);
    }
}
