#include "entity/entity.h"
#include "world/world.h"

#include <cglm/struct.h>

void movement_tick(entity_s *self, world_s *world, float dt) {
    float speed = 80.f;
    rigid_body_s *rb = &self->body;
    if (rb->direction & DIR_UP) {
        rb->vel.y = speed;
    } else if (rb->direction & DIR_DOWN) {
        rb->vel.y = -speed;
    } else {
        rb->vel.y = 0.f;
    }
    if (rb->direction & DIR_LEFT) {
        rb->vel.x = -speed;
    } else if (rb->direction & DIR_RIGHT) {
        rb->vel.x = speed;
    } else {
        rb->vel.x = 0.f;
    }

    rb->pos = glms_vec2_muladds(rb->vel, dt, rb->pos);    
}
