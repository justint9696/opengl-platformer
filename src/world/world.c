#include "world/world.h"

#include "data/array.h"

#include "graphics/ibo.h"
#include "graphics/renderer.h"
#include "graphics/vao.h"
#include "graphics/vbo.h"
#include "graphics/window.h"

#include "util/log.h"
#include "util/util.h"

#include <cglm/struct.h>
#include <math.h>
#include <string.h>

void world_init(world_t *self) {
    memset(self, 0, sizeof(world_t));

    self->vao = vao_create();
    self->vbo = vbo_create(NULL, 0);
    self->ibo = ibo_create(NULL, 0);

    chunk_init(&self->chunk);

    grid_init(&self->grid, (ivec2s) { 60.f, 60.f },
              (ivec2s) { SCREEN_WIDTH, SCREEN_HEIGHT });
}

void world_destroy(world_t *self) {
    grid_destroy(&self->grid);
    chunk_destroy(&self->chunk);
}

void world_update(world_t *self, float dt) {
    for (int i = 0; i < 9; i++) {
        page_t *page = &self->chunk.pages[i];
        size_t len = array_len(page->entities);
        for (size_t j = 0; j < len; j++) {
            entity_t *entity = &page->entities[j];
            entity_update(entity, self, dt);
        }
    }
}

void world_render(world_t *self) {
    renderer_use_shader(SHADER_DEFAULT);

    vao_bind(&self->vao);
    vbo_bind(&self->vbo);
    ibo_bind(&self->ibo);

    for (int i = 0; i < 9; i++) {
        page_t *page = &self->chunk.pages[i];
        size_t len = array_len(page->entities);
        for (size_t j = 0; j < len; j++) {
            entity_t *entity = &page->entities[j];
            entity_render(entity, self);
        }
    }

    chunk_render(&self->chunk);
}

void world_sync(world_t *self, float dt) {
    for (int i = 0; i < 9; i++) {
        page_t *page = &self->chunk.pages[i];
        size_t len = array_len(page->entities);
        for (size_t j = 0; j < len; j++) {
            entity_t *entity = &page->entities[j];
            entity_sync(entity, self, dt);
        }
    }
}

vec2s world_to_screen(const world_t *self, vec2s pos) {
    vec4s clip = glms_mat4_mulv(
        self->camera.projection,
        glms_mat4_mulv(self->camera.view, (vec4s) { pos.x, pos.y, 0.f, 1.f }));
    return (vec2s) {
        .x = roundf(((clip.x + 1) / 2.f) * SCREEN_WIDTH),
        .y = roundf(((1 - clip.y) / 2.f) * SCREEN_HEIGHT),
    };
}

vec2s screen_to_world(const world_t *self, ivec2s pos) {
    vec2s clip = (vec2s) {
        .x = (2.f * pos.x / SCREEN_WIDTH - 1),
        .y = (-2.f * pos.y / SCREEN_HEIGHT + 1),
    };

    mat4s view = glms_mat4_inv(
        glms_mat4_mul(self->camera.projection, self->camera.view));

    vec4s world = glms_mat4_mulv(view, (vec4s) { clip.x, clip.y, 0.f, 1.f });

    return (vec2s) { world.x, world.y };
}

size_t world_get_colliders(world_t *self, entity_t *entity, collider_t *arr,
                           size_t len) {
    cell_t *cell
        = grid_cell_pos(&self->grid, world_to_screen(self, entity->body.pos));
    assert(cell);

    if (!cell->items) {
        log_warn("Current cell has not been initialized.\n");
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

                arr[n++] = (collider_t) {
                    .data = e,
                    .box = e->body.box,
                    .callback = e->body.collision_callback,
                };
            }
        }
    }

    return n;
}
