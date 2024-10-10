#include "graphics/line.h"

#include "data/array.h"
#include "graphics/drawing.h"
#include "graphics/shader.h"
#include "graphics/renderer.h"
#include "util/log.h"

#include <cglm/struct.h>

static line_batch_t lines;

typedef struct vertex_s {
    vec3s pos;
    vec4s color;
} vertex_t;

void lines_init() {
    lines.items = array_alloc(sizeof(line_t), 128);

    lines.vao = vao_create();
    lines.vbo = vbo_create(NULL, sizeof(vertex_t) * 1024);
}

void lines_destroy() {
    array_free(lines.items);

    vao_destroy(&lines.vao);
    vbo_destroy(&lines.vbo);
}

void lines_push(vec2s start, vec2s end, float z, uint32_t color) {
    array_push(lines.items, &(line_t) {
        .start = start,
        .end = end,
        .z = z,
        .color = color,
    });
}

static void line_to_world(const line_t *line, vertex_t vertices[], size_t n,
                          const camera_t *camera) {
    float x = (line->end.x - line->start.x), y = (line->end.y - line->start.y);
    vertices[n]
        = (vertex_t) { .pos = { 0.f, 0.f, 0.f, }, .color = RGBA(line->color), };
    vertices[n + 1]
        = (vertex_t) { .pos = {   x,   y, 0.f, }, .color = RGBA(line->color), };

    mat4s model = glms_translate(
        GLMS_MAT4_IDENTITY,
        glms_vec3_add(camera->pos,
                      (vec3s) { line->start.x, line->start.y, line->z }));

    for (size_t i = 0; i < 2; i++) {
        vertices[i + n].pos = glms_mat4_mulv3(model, vertices[i + n].pos, 1.f);
    }
}

void lines_render(const camera_t *camera) {
    size_t len = array_len(lines.items);
    vertex_t vertices[len * 2];

    for (size_t i = 0; i < len; i++) {
        const line_t *line = &lines.items[i];
        line_to_world(line, vertices, i * 2, camera);
    }

    GLint shader = renderer_use_shader(SHADER_DEFAULT);

    vao_bind(&lines.vao);

    vbo_bind(&lines.vbo);
    vbo_buffer_sub_data(vertices, 0, sizeof(vertices));

    vao_attrib(0, 3, GL_FLOAT, sizeof(vertex_t),
               (void *)offsetof(vertex_t, pos));
    vao_attrib(1, 4, GL_FLOAT, sizeof(vertex_t),
               (void *)offsetof(vertex_t, color));

    shader_uniform_mat4f(shader, "projection", camera->projection);
    shader_uniform_mat4f(shader, "view", camera->view);

    glDrawArrays(GL_LINES, 0, len * 2);

    /* glDrawElements(GL_LINES, (len * 2), GL_UNSIGNED_INT, 0); */

    vao_unbind();
    vbo_unbind();

    array_clear(lines.items);
}
