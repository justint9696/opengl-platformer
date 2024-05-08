#include "world/world.h"
#include "data/array.h"
#include "entity/entity.h"
#include "entity/player.h"

void world_init(world_s *self) {
    self->entities = array_alloc(sizeof(entity_s), 32);
    self->player = player_create((vec2s) { 0.f, 0.f }, (vec2s) { 50.f, 50.f }, self);
}

void world_destroy(world_s *self) {
    array_free(self->entities);
}

void world_update(world_s *self, float dt) {
    size_t len = array_len(self->entities);
    for (size_t i = 0; i < len; i++) {
        entity_s *entity = array_get(self->entities, i);
        entity_update(entity, self, dt);
    }
}

void world_render(world_s *self) {
    size_t len = array_len(self->entities);
    for (size_t i = 0; i < len; i++) {
        entity_s *entity = array_get(self->entities, i);
        entity_render(entity, self);
    }
}

void world_tick(world_s *self, float dt) {
    size_t len = array_len(self->entities);
    for (size_t i = 0; i < len; i++) {
        entity_s *entity = array_get(self->entities, i);
        entity_tick(entity, self, dt);
    }
}
