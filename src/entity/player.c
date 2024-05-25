#include "entity/player.h"

#include "data/kdtree.h"
#include "entity/entity.h"
#include "game/defs.h"
#include "graphics/color.h"
#include "graphics/drawing.h"
#include "graphics/window.h"
#include "tile/tile.h"
#include "util/aabb.h"
#include "world/world.h"

#include <cglm/struct.h>

static void collision_callback(entity_t *self, entity_t *entity) {}

static void init(entity_t *self, world_t *world) {
    camera_init(&world->camera, SCREEN_WIDTH, SCREEN_HEIGHT);
}

static void tick(entity_t *self, world_t *world, float dt) {}

static void render(entity_t *self, world_t *world) {
    /* box_t bbb = self->body.box; */
    /* for (int i = 0; i < 2; i++) { */
    /*     bbb_create(&bbb, self->body.vel, i); */
    /* } */

    tile_t *tile = kdtree_nearest(&world->kdtree, self->body.pos);
    vec2s a = box_center(&self->body.box);
    vec2s b = box_center(&tile->body.box);
    draw_line(a, b, COLOR_RED);

    /* draw_quad(bbb.pos, bbb.dim, COLOR_RED); */
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
            .movement_speed = 3.f,
            .jump_speed = GRAVITY * 1.2f,
            .collision_callback = collision_callback,
        },
        .flags = F_PLAYER_CONTROLLED | F_KINEMATIC | F_GRAVITY,
    }, world);
}
