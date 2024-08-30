#include "player.h"

#include "entity/entity.h"
#include "game/defines.h"
#include "graphics/drawing.h"
#include "graphics/window.h"
#include "world/world.h"

#include <cglm/struct.h>

static inline void draw_debug(entity_t *self, world_t *world) {
    draw_debug_text("Player Position: (%.2f, %.2f)", self->body.pos.x,
                    self->body.pos.y);
    vec2s screen = world_to_screen(world, self->body.pos);
    draw_debug_text("Screen Position: (%.2f, %.2f)", screen.x, screen.y);
    draw_debug_text("Current Chunk: %d: %d", world->chunk.index,
                    self->body.page->index);
}

static void init(entity_t *self, world_t *world) {
    camera_init(&world->camera, self->body.pos, SCREEN_WIDTH, SCREEN_HEIGHT);
}

static void render(entity_t *self, world_t *world) {
    draw_quad(self->body.pos, self->body.dim, COLOR_BLUE);
    draw_debug(self, world);
}

entity_t *player_init(vec2s pos, vec2s dim, world_t *world) {
    entity_t *self = calloc(1, sizeof(entity_t));
    memcpy(self, &(entity_t) {
        .init = init,
        .render = render,
        .body = {
            .pos = pos, 
            .dim = dim,
            .solid = true,
            .mass = 3.f,
            .speed = 24.f,
            .jump_force = GRAVITY * 0.8f,
            .dash_force = 50.f,
        },
        .type = ET_PLAYER,
        .flags = EF_PLAYER_CONTROLLED 
                 | EF_KINEMATIC 
                 | EF_GRAVITY
                 | EF_CAMERA_FOLLOW,
    }, sizeof(entity_t));

    entity_init(self, world);
    return self;
}

void player_destroy(entity_t *self) {
    assert(self);
    free(self);
}

void player_update(entity_t *self, world_t *world, float dt) {
    entity_update(self, world, dt);
}

void player_sync(entity_t *self, world_t *world, float dt) {
    entity_sync(self, world, dt);
}

void player_render(entity_t *self, world_t *world) {
    entity_render(self, world);
}
