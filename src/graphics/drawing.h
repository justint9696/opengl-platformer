/**
 * @file drawing.h
 * @author Justin Tonkinson
 * @date 2024/05/06
 * @brief Drawing function prototypes with included headers for vertex buffer
 * objects.
 */

#ifndef _GRAPHICS_DRAWING_H_
#define _GRAPHICS_DRAWING_H_

#include "graphics/color.h"
#include "graphics/ibo.h"
#include "graphics/vao.h"
#include "graphics/vbo.h"

#include <cglm/types-struct.h>
#include <stdint.h>

/**
 * @brief Renders a quad at the provided world position.
 * @param pos world position of the quad
 * @param dim dimensions of the quad
 * @param color rgba color of the quad
 */
extern void draw_quad(vec2s pos, vec2s dim, uint32_t color);

/**
 * @brief Renders a line from the starting and ending world positions.
 * @param start starting point of the line in world space
 * @param end ending point of the line in world space
 * @param color rgba color of the line
 */
extern void draw_line(vec2s start, vec2s end, uint32_t color);

/**
 * @brief Renders an unfilled quad at the provided world position.
 * @param pos world position of the quad
 * @param dim dimensions of the quad
 * @param color rgba color of the quad
 */
extern void draw_quad_line(vec2s pos, vec2s dim, uint32_t color);

/**
 * @brief Renders text at the provided screen coordinates.
 * @param pos screen position of the text
 * @param scale text scale amount
 * @param color rgba color of the text
 * @param format printf format string
 */
extern void draw_text(vec2s pos, float scale, uint32_t color,
                      const char *format, ...);

/**
 * @brief Renders debug text at the top left corner of the screen.
 * @param format printf format string
 */
extern void draw_debug_text(const char *format, ...);

#endif // ifndef _GRAPHICS_DRAWING_H_
