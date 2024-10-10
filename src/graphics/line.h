/**
 * @file line.h
 * @author Justin Tonkinson
 * @date 2024/10/09
 * @brief This file contains the data structures and function prototypes needed
 * for line batching.
 */

#ifndef _GRAPHICS_LINE_H_
#define _GRAPHICS_LINE_H_

#include "game/camera.h"

#include <cglm/types-struct.h>
#include <glad/glad.h>
#include <stdint.h>

/** @brief Line data structure. */
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

/** @brief Line bach data structure. */
typedef struct line_batch_s {
    /** @brief Dynamic array of lines to be rendered. */
    line_t *arr;

    /** @brief Vertex buffer objects. */
    GLuint vao, vbo;
} line_batch_t;

/**
 * @brief Initializes the line batch internal state.
 * @param self a pointer to a line batch
 */
void lines_init();

/**
 * @brief Destroys the line batch internal state.
 * @param self a pointer to a line batch
 */
void lines_destroy();

/**
 * @brief Creats a line and adds it to the batch.
 * @param self a pointer to a line batch
 * @param pos position of the line
 * @param dim dimensions of the line
 * @param z z position of the line
 * @param color color of the line
 */
void lines_push(vec2s start, vec2s end, float z, uint32_t color);

/**
 * @brief Renders the line batch and clears its contents.
 * @param self a pointer to a line batch
 * @param camera a pointer to the scene camera
 */
void lines_render(const camera_t *);

#endif // ifndef _GRAPHICS_LINE_H_
