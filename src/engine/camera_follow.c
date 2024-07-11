#include "entity/entity.h"
#include "game/camera.h"
#include "world/world.h"

#include <cglm/struct.h>

void camera_follow_sync(entity_t *self, world_t *world, float dt) {
    world->camera.pos
        = glms_vec3_negate((vec3s) { self->body.pos.x, self->body.pos.y, 0.f });
}
