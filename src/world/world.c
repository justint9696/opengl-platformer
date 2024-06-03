#include "world/world.h"

#include "data/array.h"
#include "data/kdtree.h"
#include "entity/entity.h"
#include "entity/player.h"
#include "graphics/window.h"
#include "tile/platform.h"
#include "tile/tile.h"
#include "util/io.h"
#include "util/util.h"

#include <cglm/struct.h>
#include <math.h>
#include <string.h>

static void create_platforms(world_t *self) {
    float size = 50.f;
    float pos_y = -(SCREEN_HEIGHT * 0.5f) + (size * 0.5f);
    vec2s dim = (vec2s) { size, size };
    platform_create((vec2s) { 0.f, pos_y }, dim, self);
    platform_create((vec2s) { -size, pos_y }, dim, self);
    platform_create((vec2s) { +size, pos_y }, dim, self);
    platform_create((vec2s) { -size * 4, pos_y }, dim, self);
    platform_create((vec2s) { -size * 5, pos_y }, dim, self);
    platform_create((vec2s) { 0.f, pos_y + (size * 2) }, dim, self);
    platform_create((vec2s) { -size * 3, pos_y + (size * 4) }, dim, self);
    platform_create((vec2s) { +size * 2, pos_y + size }, dim, self);
    platform_create((vec2s) { +size * 2, pos_y + (size * 3) }, dim, self);
    platform_create((vec2s) { -size * 2, pos_y + size }, dim, self);
    /* platform_create((vec2s) { -size * 2, pos_y + (size * 2) }, dim, self); */
}

void world_init(world_t *self) {
    memset(self, 0, sizeof(world_t));
    grid_init(&self->grid, (ivec2s) { 60.f, 60.f },
              (ivec2s) { SCREEN_WIDTH, SCREEN_HEIGHT });
    self->entities = array_alloc(sizeof(entity_t), 64);
    self->tiles = array_alloc(sizeof(tile_t), 64);
    self->player
        = player_create((vec2s) { 0.f, 0.f }, (vec2s) { 50.f, 50.f }, self);
    /* enemy_create((vec2s) { 30.f, 80.f }, (vec2s) { 50.f, 50.f }, self); */

    create_platforms(self);
    
    size_t len = array_len(self->tiles);
    int offset = offsetof(tile_t, body.pos);
    kdtree_from(&self->kdtree, self->tiles, len, offset, tile_sort);
}

void world_destroy(world_t *self) {
    grid_destroy(&self->grid);
    array_free(self->entities);
    array_free(self->tiles);
}

void world_update(world_t *self, float dt) {
    size_t len = array_len(self->entities);
    for (size_t i = 0; i < len; i++) {
        entity_t *entity = &self->entities[i];
        entity_update(entity, self, dt);
    }
}

void world_render(world_t *self) {
    size_t len = array_len(self->entities);
    for (size_t i = 0; i < len; i++) {
        entity_t *entity = &self->entities[i];
        entity_render(entity, self);
    }

    len = array_len(self->tiles);
    for (size_t i = 0; i < len; i++) {
        tile_t *tile = &self->tiles[i];
        tile_render(tile, self);
    }
}

void world_tick(world_t *self, float dt) {
    size_t len = array_len(self->entities);
    for (size_t i = 0; i < len; i++) {
        entity_t *entity = &self->entities[i];
        entity_tick(entity, self, dt);
    }
}

vec2s world_to_screen(const world_t *self, vec2s pos) {
    vec4s clip = glms_mat4_mulv(
        self->camera.projection,
        glms_mat4_mulv(self->camera.view, (vec4s) { pos.x, pos.y, 0.f, 1.f }));
    return (vec2s) {
        .x = roundf(((clip.x + 1) / 2.f) * SCREEN_WIDTH),
        .y = roundf(((clip.y + 1) / 2.f) * SCREEN_HEIGHT),
    };
}

size_t world_get_colliders(
        world_t *self, entity_t *entity, entity_t *arr[], size_t len) {
    cell_t *cell
        = grid_cell_pos(&self->grid, world_to_screen(self, entity->body.pos));
    assert(cell);

    if (!cell->items) {
        WARN(0, "Current cell has not been initialized.\n");
        return 0;
    }

    size_t n = 0;
    ivec2s pos = cell->pos, dim = self->grid.dim;
    for (int y = max(pos.y - 1, 0); y <= min(pos.y + 1, dim.y); y++) {
        for (int x = max(pos.x - 1, 0); x <= min(pos.x + 1, dim.x); x++) {
            cell_t *tmp = grid_cell_index(&self->grid, (ivec2s) { x, y });
            assert(tmp);

            if (!tmp->items)
                continue;

            for (size_t i = 0; i < array_len(tmp->items); i++) {
                entity_t *e = *(entity_t **)array_get(tmp->items, i);
                assert(e);

                if (entity == e)
                    continue;

                arr[n++] = e;
            }
        }
    }

    return n;
}
