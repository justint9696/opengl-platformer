/**
 * @file renderer.c
 * @author Justin Tonkinson
 * @date 2024/05/06
 * @brief Renderer implementation functions.
 */

#include "graphics/renderer.h"

#include "graphics/drawing.h"
#include "graphics/font.h"
#include "graphics/line.h"
#include "graphics/sprite.h"
#include "graphics/window.h"
#include "ui/ui.h"
#include "util/log.h"

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

    /* for (size_t i = 0; i < SHADER_MAX; i++) { */
    /*     renderer.batch[i].indices = batch_alloc(1024); */
    /*     renderer.batch[i].vertices = batch_alloc(1024); */
    /* } */

    sprites_init();
    lines_init();

    // load textures
    /* renderer.texture = texture_create("assets/images/white.png"); */

    // load text
    font_init("assets/fonts/helvetica.ttf");
}

void renderer_destroy() {
    // destroy loaded shaders
    for (size_t i = 0; i < SHADER_MAX; i++) {
        shader_destroy(renderer.shaders[i]);
    }

    sprites_destroy();
    lines_destroy();

    // destroy textures
    /* texture_destroy(renderer.texture); */

    // destroy font textures
    font_destroy();
}

GLint renderer_use_shader(shader_type_t index) {
    const GLint shader = renderer.shaders[index];
    if (shader == renderer.shader)
        return shader;

    renderer.shader = shader;
    shader_use(shader);

    return shader;
}

void renderer_prepare_scene(const camera_t *camera) {
    renderer.camera = camera;
    glClear(GL_COLOR_BUFFER_BIT);
}

void renderer_present_scene() {
    sprites_render(renderer.camera);
    lines_render(renderer.camera);
}

void draw_quad(vec2s pos, vec2s dim, uint32_t color) {
    sprites_push(pos, dim, 0.f, color);
}

void draw_line(vec2s start, vec2s end, uint32_t color) {
    lines_push(start, end, 0.f, color);
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

void draw_debug_text(const char *format, ...) {
    va_list arg;
    char text[64];

    va_start(arg, format);
    vsnprintf(text, 64, format, arg);
    va_end(arg);

    ui_draw_debug(text);
}
