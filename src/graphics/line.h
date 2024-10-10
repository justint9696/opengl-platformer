#ifndef _GRAPHICS_LINE_H_
#define _GRAPHICS_LINE_H_

#include "game/camera.h"

#include <cglm/types-struct.h>
#include <glad/glad.h>
#include <stdint.h>

typedef struct line_s {
    /** @brief The starting position of the line. */
    vec2s start;

    /** @brief The ending positions of the line. */
    vec2s end;

    /** @brief The z position of the line. */
    float z;

    /** @brief The color (RGBA) of the line. */
    uint32_t color;
} line_t;

typedef struct line_batch_s {
    /** @brief Dynamic array of lines to be rendered. */
    line_t *items;

    /** @brief Vertex buffer objects. */
    GLuint vao, vbo;
} line_batch_t;

void lines_init();

void lines_destroy();

void lines_push(vec2s start, vec2s end, float z, uint32_t color);

void lines_render(const camera_t *);

#endif // ifndef _GRAPHICS_LINE_H_
