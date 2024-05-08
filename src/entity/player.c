#include "entity/player.h"
#include "entity/entity.h"
#include "graphics/color.h"
#include "graphics/drawing.h"
#include "world/world.h"

static void render(entity_s *self, world_s *world) {
    draw_quad(self->body.pos, self->body.dim, COLOR_BLUE);
}

entity_s *player_create(vec2s pos, vec2s dim, world_s *world) {
    return entity_create(&(entity_s) {
        .render = render,
        .body = {
            .pos = pos, 
            .dim = dim,
        },
        .flags = FLAG_PLAYER_INPUT | FLAG_KINEMATIC,
    }, world);
}
