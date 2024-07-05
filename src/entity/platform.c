#include "entity/platform.h"

#include "entity/entity.h"
#include "graphics/drawing.h"
#include "world/world.h"

static void collision_callback(entity_t *self, entity_t *entity) {}

static void render(entity_t *self, world_t *world) {
    draw_quad(self->body.pos, self->body.dim, COLOR_WHITE);
}

entity_t *platform_create(vec2s pos, vec2s dim, world_t *world) {
    return entity_create(&(entity_t) {
        .render = render,
        .body = {
            .pos = pos, 
            .dim = dim,
            .solid = true,
            .collision_callback = collision_callback,
        },
        .type = ET_PLATFORM,
    }, world);
}
