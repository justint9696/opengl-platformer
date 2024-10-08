#include "graphics/sprite.h"

#include "data/array.h"
#include "graphics/drawing.h"
#include "graphics/shader.h"

#include <cglm/struct.h>
#include <string.h>

/** @brief Sprite vertex attributes layout. */
typedef struct vertex_s {
    vec3s pos;
    vec4s color;
} vertex_t;

static const uint32_t SPRITE_INDICES[6];
static const vertex_t SPRITE_VERTICES[4];

void sprite_init(sprite_batch_t *self) {
    self->sprites = array_alloc(sizeof(sprite_t), 128);

    self->vao = vao_create();
    self->vbo = vbo_create(NULL, sizeof(vertex_t) * 1024);
    self->ibo = ibo_create(NULL, sizeof(uint32_t) * 1024);

    self->shader = shader_create("shaders/default.vs", "shaders/default.fs");
}

void sprite_destroy(sprite_batch_t *self) {
    array_free(self->sprites);

    vao_destroy(&self->vao);
    vbo_destroy(&self->vbo);
    ibo_destroy(&self->ibo);

    shader_destroy(self->shader);
}

void sprite_push(sprite_batch_t *self, vec2s pos, vec2s dim, float z,
                 uint32_t color) {
    array_push(self->sprites,
               &(sprite_t) { .pos = pos, .dim = dim, .z = z, .color = color, });
}

static void sprite_to_world(const sprite_t *sprite, vertex_t vertices[],
                            size_t n, const camera_t *camera) {
    mat4s model = glms_translate(
        GLMS_MAT4_IDENTITY,
        glms_vec3_add(camera->pos,
                      (vec3s) { sprite->pos.x, sprite->pos.y, sprite->z }));

    model = glms_scale(model,
                       (vec3s) { sprite->dim.x, sprite->dim.y, 1.f });

    memcpy(&vertices[n], SPRITE_VERTICES, sizeof(SPRITE_VERTICES));
    for (size_t i = n; i < (n + 4); i++) {
        vertex_t *vertex = &vertices[i];
        vertex->color = RGBA(sprite->color);
        vertex->pos = glms_mat4_mulv3(model, vertex->pos, 1.f);
    }
}

void sprite_render(sprite_batch_t *self, const camera_t *camera) {
    size_t len = array_len(self->sprites);
    uint32_t indices[len * 6];
    vertex_t vertices[len * 4];

    for (size_t i = 0; i < len; i++) {
        const sprite_t *sprite = &self->sprites[i];

        for (size_t j = 0; j < 6; j++) {
            uint32_t idx = SPRITE_INDICES[j] + (i * 4);
            indices[(i * 6) + j] = idx;
        }

        sprite_to_world(sprite, vertices, (i * 4), camera);
    }

    shader_use(self->shader);

    vao_bind(&self->vao);
    vao_attrib(0, 3, GL_FLOAT, sizeof(vertex_t),
               (void *)offsetof(vertex_t, pos));
    vao_attrib(1, 4, GL_FLOAT, sizeof(vertex_t),
               (void *)offsetof(vertex_t, color));

    vbo_bind(&self->vbo);
    vbo_buffer_sub_data(vertices, 0, sizeof(vertices));

    ibo_bind(&self->ibo);
    ibo_buffer_sub_data(indices, 0, sizeof(indices));

    shader_uniform_mat4f(self->shader, "projection", camera->projection);
    shader_uniform_mat4f(self->shader, "view", camera->view);

    glDrawElements(GL_TRIANGLES, (len * 6), GL_UNSIGNED_INT, 0);

    array_clear(self->sprites);
}

static const uint32_t SPRITE_INDICES[6] = {
    0, 1, 3,
    1, 2, 3,
};

static const vertex_t SPRITE_VERTICES[4] = {
    { .pos = { 0.f, 0.f, 0.f }, .color = { 0.f, 0.f, 0.f, 0.f }, },
    { .pos = { 1.f, 0.f, 0.f }, .color = { 0.f, 0.f, 0.f, 0.f }, },
    { .pos = { 1.f, 1.f, 0.f }, .color = { 0.f, 0.f, 0.f, 0.f }, },
    { .pos = { 0.f, 1.f, 0.f }, .color = { 0.f, 0.f, 0.f, 0.f }, },
};
