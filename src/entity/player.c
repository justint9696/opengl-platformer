#include "entity/player.h"
#include "entity/entity.h"
#include "graphics/color.h"
#include "graphics/drawing.h"
#include "world/world.h"

#include <stdio.h>

static void init(entity_s *self, world_s *world) {
    printf("player initialized\n");
}

static void render(entity_s *self, world_s *world) {
    draw_quad(self->body.pos, self->body.dim, COLOR_BLUE);
}

entity_s *player_create(vec2s pos, vec2s dim, world_s *world) {
    return entity_create(&(entity_s) {
        .init = init,
        .render = render,
        .body = {
            .pos = pos, 
            .dim = dim,
        },
    }, world);
}
