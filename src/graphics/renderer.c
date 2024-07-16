#include "graphics/renderer.h"

#include "graphics/drawing.h"
#include "graphics/font.h"
#include "graphics/ibo.h"
#include "graphics/vao.h"
#include "graphics/vbo.h"
#include "graphics/window.h"
#include "ui/ui.h"

#include <cglm/struct.h>
#include <stdio.h>
#include <string.h>

static renderer_t renderer;

void renderer_init() {
    memset(&renderer, 0, sizeof(renderer_t));

    glClearColor(0.53f, 0.81f, 0.92f, 1.f);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // load shaders
    renderer.shaders[SHADER_DEFAULT] 
        = shader_create("shaders/default.vs", "shaders/default.fs");
    renderer.shaders[SHADER_UI_TEXT] 
        = shader_create("shaders/text2d.vs", "shaders/text2d.fs");
    renderer.shaders[SHADER_UI_TEXTURE] 
        = shader_create("shaders/texture2d.vs", "shaders/texture2d.fs");

    // load text
    font_init("assets/fonts/helvetica.ttf"); 
}

void renderer_destroy() {
    for (size_t i = 0; i < SHADER_MAX; i++) {
        shader_destroy(renderer.shaders[i]);
    }
}

shader_t *renderer_use_shader(shader_e index) {
    shader_t *shader = &renderer.shaders[index];
    if (shader == renderer.shader)
        return renderer.shader;

    renderer.shader = shader;
    shader_use(renderer.shaders[index]);
    return shader;
}

void renderer_prepare_scene(const camera_t *camera) {
    renderer.camera = camera;
    glClear(GL_COLOR_BUFFER_BIT);
}

// TODO
void renderer_present_scene() {}

void draw_quad(vec2s pos, vec2s dim, uint32_t color) {
    float vertices[] = {
        0.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        1.f, 1.f, 0.f,
        1.f, 0.f, 0.f,
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    mat4s model = GLMS_MAT4_IDENTITY_INIT;
    model = glms_translate(model, 
                glms_vec3_add(
                    renderer.camera->pos, (vec3s) { pos.x, pos.y, 0.f }));

    model = glms_scale(model, (vec3s) { dim.x, dim.y, 1.f });

    vao_attrib(0, 3, GL_FLOAT, 3 * sizeof(float), NULL);

    ibo_buffer_data(indices, sizeof(indices));
    vbo_buffer_data(vertices, sizeof(vertices));

    shader_uniform_mat4f(*renderer.shader, "model", model);
    shader_uniform_mat4f(*renderer.shader, "projection", 
                         renderer.camera->projection);
    shader_uniform_mat4f(*renderer.shader, "view", renderer.camera->view);

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
    model = glms_translate(model, 
                glms_vec3_add(
                    renderer.camera->pos, (vec3s) { start.x, start.y, 0.f }));

    vbo_buffer_data(vertices, sizeof(vertices));
    shader_uniform_mat4f(*renderer.shader, "model", model);
    shader_uniform_mat4f(*renderer.shader, "projection", 
                         renderer.camera->projection);
    shader_uniform_mat4f(*renderer.shader, "view", renderer.camera->view);

    shader_uniform_vec4f(*renderer.shader, "color", RGBA(color));

    glDrawArrays(GL_LINES, 0, 2);
}

void draw_quad_line(vec2s pos, vec2s size, uint32_t color) {
    vec2s lines[] = {
        { pos.x, pos.y },
        { pos.x, pos.y + size.y },
        { pos.x + size.x, pos.y + size.y },
        { pos.x + size.x, pos.y },
    };

    for (size_t i = 0; i < 4; i++) {
        draw_line(lines[i], lines[(i+1) % 4], color);
    }
}

void draw_text(vec2s pos, float scale, uint32_t color, 
               const char *format, ...) {
    va_list arg;
    char text[64];

    va_start(arg, format);
    vsnprintf(text, 64, format, arg);
    va_end(arg);

    ui_draw_text(pos, scale, color, text);
}
