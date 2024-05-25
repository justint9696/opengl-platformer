#include "entity/enemy.h"

#include "entity/entity.h"
#include "graphics/color.h"
#include "graphics/drawing.h"
#include "world/world.h"

#include <cglm/struct.h>

static void render(entity_t *self, world_t *world) {
    draw_quad(self->body.pos, self->body.dim, COLOR_RED);
}

entity_t *enemy_create(vec2s pos, vec2s dim, world_t *world) {
    return entity_create(&(entity_t) {
        .render = render,
        .body = {
            .pos = pos, 
            .dim = dim,
            .solid = true,
            .movement_speed = 5.25f,
        },
        .flags = F_AI_CONTROLLED | F_KINEMATIC,
    }, world);
}