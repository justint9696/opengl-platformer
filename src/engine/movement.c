#include "entity/entity.h"
#include "world/chunk.h"
#include "world/world.h"

#include <cglm/struct.h>

void movement_sync(entity_t *self, world_t *world, float dt) {
    // apply movement
    self->body.pos = glms_vec2_muladds(self->body.vel, dt, self->body.pos);

    // reset force vector
    self->body.force = GLMS_VEC2_ZERO;

    /* page_t *page = chunk_page_from_pos(&world->chunk, self->body.pos); */
    /* if (page == self->body.page) */
    /*     return; */

    /* self->body.page = page; */
}
