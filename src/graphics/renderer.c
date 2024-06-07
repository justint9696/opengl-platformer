#include "graphics/renderer.h"

#include "graphics/drawing.h"
#include "graphics/shader.h"
#include "graphics/vao.h"
#include "graphics/window.h"

#include <cglm/struct.h>
#include <string.h>

static renderer_t renderer;

const float QUAD_VERTICES[12] = {
    0.f, 0.f, 0.f,
    0.f, 1.f, 0.f,
    1.f, 1.f, 0.f,
    1.f, 0.f, 0.f,
};

const unsigned int QUAD_INDICES[6] = {
    0, 1, 3,
    1, 2, 3
};

void renderer_init() {
    memset(&renderer, 0, sizeof(renderer_t));

    glClearColor(0.53f, 0.81f, 0.92f, 1.f);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // load shaders
    renderer.shaders[0] 
        = shader_create("shaders/default2d.vs", "shaders/default2d.fs");
    renderer.shaders[1] = shader_create("shaders/text2d.vs", "shaders/text2d.fs");
}

void renderer_destroy() {
    for (size_t i = 0; i < SHADER_MAX; i++) {
        shader_destroy(renderer.shaders[i]);
    }
}

void renderer_use_shader(shadertype_t index) {
    shader_t *shader = &renderer.shaders[index];
    if (!memcmp(shader, &renderer.shader, sizeof(shader_t)))
        return;

    renderer.shader = shader;
    shader_use(renderer.shaders[index]);
}

void renderer_prepare_scene(const camera_t *camera) {
    renderer.camera = camera;
    glClear(GL_COLOR_BUFFER_BIT);
}

void renderer_present_scene() {
    shader_uniform_mat4f(
            *renderer.shader, "projection", renderer.camera->projection);
    shader_uniform_mat4f(
            *renderer.shader, "view", renderer.camera->view);
}

void draw_quad(vec2s pos, vec2s dim, uint32_t color) {
    mat4s model = GLMS_MAT4_IDENTITY_INIT;
    model = glms_translate(model, (vec3s) { pos.x, pos.y, 0.f });
    model = glms_scale(model, (vec3s) { dim.x, dim.y, 1.f });

    vao_attrib(0, 3, GL_FLOAT, 3 * sizeof(float), NULL);

    shader_uniform_mat4f(*renderer.shader, "model", model);
    shader_uniform_vec4f(*renderer.shader, "color", RGBA(color));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void draw_line(vec2s start, vec2s end, uint32_t color) {
    float x = (end.x - start.x), y = (end.y - start.y);
    float vertices[] = {
        0.f, 0.f, 0.f, 
          x,   y, 0.f,
    };

    mat4s model = GLMS_MAT4_IDENTITY_INIT;
    model = glms_translate(model, (vec3s) { start.x, start.y, 0.f });

    shader_uniform_mat4f(*renderer.shader, "model", model);
    shader_uniform_vec4f(*renderer.shader, "color", RGBA(color));

    glDrawArrays(GL_LINES, 0, 2);
}
