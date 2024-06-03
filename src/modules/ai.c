#include "entity/entity.h"
#include "game/types.h"
#include "world/world.h"

void ai_tick(entity_t *self, world_t *world, float dt) {
    if (self->body.pos.x <= -270) {
        self->body.direction.x = 1.f; 
    } else if (self->body.pos.x >= 270 - self->body.dim.x) {
        self->body.direction.x = -1.f; 
    } else if (self->body.vel.x == 0.f) {
        self->body.direction.x = -1.f; 
    }
}
