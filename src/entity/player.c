#include "entity/player.h"

#include "entity/entity.h"
#include "game/defines.h"
#include "graphics/drawing.h"
#include "graphics/window.h"
#include "world/world.h"

#include <cglm/struct.h>

static inline void draw_debug(entity_t *self, world_t *world) {
    draw_text((vec2s) { -315, 220 }, 1.f, COLOR_WHITE,
              "Player Position: (%.2f, %.2f)", self->body.pos.x,
              self->body.pos.y);
    vec2s screen = world_to_screen(world, self->body.pos);
    draw_text((vec2s) { -315, 200 }, 1.f, COLOR_WHITE,
              "Screen Position: (%.2f, %.2f)", screen.x, screen.y);
    draw_text((vec2s) { -315, 180 }, 1.f, COLOR_WHITE, "Current Chunk: %d",
              world->chunk.index);
}

static void collision_callback(entity_t *self, entity_t *entity) {}

static void init(entity_t *self, world_t *world) {
    camera_init(&world->camera, self->body.pos, SCREEN_WIDTH, SCREEN_HEIGHT);
}

static void sync(entity_t *self, world_t *world, float dt) {}

static void render(entity_t *self, world_t *world) {
    draw_quad(self->body.pos, self->body.dim, COLOR_BLUE);
    draw_debug(self, world);
}

entity_t *player_create(vec2s pos, vec2s dim, world_t *world) {
    return entity_create(&(entity_t) {
        .init = init,
        .render = render,
        .sync = sync,
        .body = {
            .pos = pos, 
            .dim = dim,
            .solid = true,
            .mass = 3.f,
            .speed = 24.f,
            .jump_force = GRAVITY * 0.8f,
            .dash_force = 50.f,
            .collision_callback = collision_callback,
        },
        .type = ET_PLAYER,
        .flags = F_PLAYER_CONTROLLED 
                 | F_KINEMATIC 
                 | F_GRAVITY
                 | F_CAMERA_FOLLOW,
    }, world);
}
