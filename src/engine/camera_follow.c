#include "entity/entity.h"
#include "level/level.h"
#include "world/world.h"

#include <cglm/struct.h>

void camera_follow_sync(entity_t *self, world_t *world, float dt) {
    world->camera.pos = glms_vec3_lerp(
        world->camera.pos,
        glms_vec3_negate((vec3s) { self->body.pos.x, self->body.pos.y, 0.f }),
        dt);

    if (!aabb_collision_2d(&self->body.box, &self->body.page->box)) {
        const page_t *page = chunk_page_from_pos(&world->chunk, self->body.pos);
        level_swap_chunks(world, (page->index - self->body.page->index));
    }
}
