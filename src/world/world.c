/**
 * @brief world.c
 * @author Justin Tonkinson
 * @date 2024/07/04
 * @brief World implementation functions.
 */

#include "world/world.h"

#include "data/array.h"

#include "graphics/drawing.h"
#include "graphics/renderer.h"
#include "graphics/window.h"

#include "player.h"

#include <cglm/struct.h>
#include <string.h>

/* static void spawn_platforms(world_t *self, size_t count, vec2s pos) { */
/*     for (size_t i = 0; i < count; i++) { */
/*         platform_create(pos, (vec2s) { 50.f, 50.f }, self); */
/*         pos.x += 50.f; */
/*     } */
/* } */

void world_init(world_t *self) {
    memset(self, 0, sizeof(world_t));

    self->vao = vao_create();
    self->vbo = vbo_create(NULL, 0);
    self->ibo = ibo_create(NULL, 0);

    chunk_init(&self->chunk);
    grid_init(&self->grid, 60, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void world_destroy(world_t *self) {
    chunk_destroy(&self->chunk);

    vao_destroy(&self->vao);
    vbo_destroy(&self->vbo);
    ibo_destroy(&self->vbo);
}

void world_update(world_t *self, float dt) {
    player_update(self->player, self, dt);

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
    {
        player_render(self->player, self);

        for (int i = 0; i < 9; i++) {
            page_t *page = &self->chunk.pages[i];
            /* page_t *page = &self->chunk.pages[4]; */
            size_t len = array_len(page->entities);
            for (size_t j = 0; j < len; j++) {
                entity_t *entity = &page->entities[j];
                entity_render(entity, self);
            }
        }

        chunk_render(&self->chunk);
    }
    vao_unbind();
    vbo_unbind();
    ibo_unbind();
}

void world_sync(world_t *self, float dt) {
    player_sync(self->player, self, dt);

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
    vec3s wpos = glms_vec3_add(self->camera.pos, (vec3s) { pos.x, pos.y, 0.f });
    vec4s clip = glms_mat4_mulv(
        self->camera.projection,
        glms_mat4_mulv(self->camera.view,
                       (vec4s) { wpos.x, wpos.y, wpos.z, 1.f }));
    return (vec2s) {
        .x = (((clip.x + 1) / 2.f) * SCREEN_WIDTH),
        .y = (((clip.y + 1) / 2.f) * SCREEN_HEIGHT),
    };
}

vec2s screen_to_world(const world_t *self, ivec2s pos) {
    vec4s clip = (vec4s) {
        .x = (2.f * pos.x / SCREEN_WIDTH - 1),
        .y = (-2.f * pos.y / SCREEN_HEIGHT + 1),
        .z = 0.f,
        .w = 1.f,
    };

    mat4s view = glms_mat4_inv(
        glms_mat4_mul(self->camera.projection, self->camera.view));

    vec4s camera = ((vec4s) { self->camera.pos.x, self->camera.pos.y,
                              self->camera.pos.z, 1.f });

    vec4s world = glms_vec4_sub(glms_mat4_mulv(view, clip), camera);

    return (vec2s) { world.x, world.y };
}

bool world_is_on_screen(const world_t *self, vec2s pos) {
    vec2s screen_pos = world_to_screen(self, pos);
    return ((screen_pos.x > 0 && screen_pos.x < SCREEN_WIDTH)
            && (screen_pos.y > 0 && screen_pos.y < SCREEN_HEIGHT));
}

size_t world_get_colliders(world_t *self, entity_t *entity, collider_t arr[],
                           size_t len) {
    cell_t *cell = grid_cell_from_pos(&self->grid,
                                      world_to_screen(self, entity->body.pos));
    if (!cell)
        return 0;

    size_t n = 0;
    ivec2s pos = cell->pos, dim = self->grid.dim;
    for (int y = glm_max(pos.y - 1, 0); y <= glm_min(pos.y + 1, dim.y); y++) {
        for (int x = glm_max(pos.x - 1, 0); x <= glm_min(pos.x + 1, dim.x);
             x++) {
            cell_t *tmp = grid_cell_from_index(&self->grid, (ivec2s) { x, y });
            for (size_t i = 0; i < tmp->len; i++) {
                entity_t *e = tmp->arr[i];
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
