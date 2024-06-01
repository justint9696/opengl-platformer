#include "tile/platform.h"

#include "entity/entity.h"
#include "graphics/drawing.h"
#include "world/world.h"

#include <stdio.h>

static void collision_callback(tile_t *self, entity_t *entity) {
    /* printf("platform collision\n"); */
}

static void render(tile_t *self, world_t *world) {
    draw_quad(self->body.pos, self->body.dim, COLOR_WHITE);
}

tile_t *platform_create(vec2s pos, vec2s dim, world_t *world) {
    return tile_create(&(tile_t) {
        .render = render,
        .body = {
            .pos = pos, 
            .dim = dim,
            .collision_callback = collision_callback,
        },
    }, world);
}
