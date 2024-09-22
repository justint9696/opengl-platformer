#ifndef _UI_UI_H_
#define _UI_UI_H_

#include "data/queue.h"
#include "game/camera.h"
#include "graphics/shader.h"
#include "graphics/window.h"

#include <cglm/types-struct.h>
#include <glad/glad.h>
#include <stdint.h>

#define UI_MAX 32

#define UI_DEBUG_HORZ ((-SCREEN_WIDTH / 2.f) + 5.f)
#define UI_DEBUG_VERT ((SCREEN_HEIGHT / 2.f) - 20.f)

typedef struct {
    vec2s pos;
    vec2s dim;
    uint32_t color;
} ui_texture_t;

typedef struct {
    char text[64];
    vec2s pos;
    float scale;
    uint32_t color;
} ui_text_t;

typedef struct {
    struct {
        size_t n;
        queue_t items;
        GLuint vao, vbo;
    } text;

    struct {
        queue_t items;
        GLuint vao, vbo, ibo;
    } texture;

    const shader_t *shader;
} ui_t;

void ui_init();
void ui_destroy();

void ui_render(const camera_t *);
void ui_clear();

void ui_draw_debug(const char *text);
void ui_draw_text(vec2s pos, float scale, uint32_t color, const char *text);
void ui_draw_quad(vec2s pos, vec2s dim, uint32_t color);

void ui_draw_quad(vec2s pos, vec2s dim, uint32_t color);

#endif // ifndef _UI_UI_H_
