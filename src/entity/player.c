#include "entity/player.h"

#include "entity/entity.h"
#include "game/defs.h"
#include "graphics/color.h"
#include "graphics/drawing.h"
#include "graphics/window.h"
#include "world/world.h"

#include <cglm/struct.h>

static void collision_callback(entity_t *self, entity_t *entity) {}

static void init(entity_t *self, world_t *world) {
    camera_init(&world->camera, SCREEN_WIDTH, SCREEN_HEIGHT);
}

static void tick(entity_t *self, world_t *world, float dt) {}

static void render(entity_t *self, world_t *world) {
    draw_quad(self->body.pos, self->body.dim, COLOR_BLUE);
}

entity_t *player_create(vec2s pos, vec2s dim, world_t *world) {
    return entity_create(&(entity_t) {
        .init = init,
        .tick = tick,
        .render = render,
        .body = {
            .pos = pos, 
            .dim = dim,
            .solid = true,
            .mass = 3.f,
            .movement_speed = 18.f,
            .jump_speed = GRAVITY * 0.8f,
            .collision_callback = collision_callback,
        },
        .flags = F_PLAYER_CONTROLLED | F_KINEMATIC | F_GRAVITY,
    }, world);
}
