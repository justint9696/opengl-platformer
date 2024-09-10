#ifndef _GRAPHICS_DRAWING_H_
#define _GRAPHICS_DRAWING_H_

#include "graphics/color.h"
#include "graphics/ibo.h"
#include "graphics/vao.h"
#include "graphics/vbo.h"

#include <cglm/types-struct.h>
#include <stdint.h>

/* Renders a quad at the provided world position. */
extern void draw_quad(vec2s pos, vec2s dim, uint32_t color);

/* Renders a line from the starting and ending world positions. */
extern void draw_line(vec2s start, vec2s end, uint32_t color);

/* Renders an unfilled quad at the provided world position. */
extern void draw_quad_line(vec2s pos, vec2s dim, uint32_t color);

/* Renders text at the provided screen coordinates. */
extern void draw_text(vec2s pos, float scale, uint32_t color,
                      const char *format, ...);

/* Renders debug text at the top left corner of the screen. */
extern void draw_debug_text(const char *format, ...);

#endif // ifndef _GRAPHICS_DRAWING_H_
