#include "entity/player.h"

#include "entity/entity.h"
#include "game/constants.h"
#include "graphics/drawing.h"
#include "graphics/window.h"
#include "util/log.h"
#include "world/world.h"

#include <cglm/struct.h>

static void collision_callback(entity_t *self, entity_t *entity) {}

static void init(entity_t *self, world_t *world) {
    camera_init(&world->camera, self->body.pos, SCREEN_WIDTH, SCREEN_HEIGHT);
    log_debug("player belongs to page %d\n", self->body.page->index);
}

static void sync(entity_t *self, world_t *world, float dt) {}

static void render(entity_t *self, world_t *world) {
    draw_quad(self->body.pos, self->body.dim, COLOR_BLUE);
    draw_quad(self->body.page->box.pos, self->body.page->box.dim, COLOR_WHITE);
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
            .speed = 18.f,
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
