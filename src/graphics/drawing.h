#ifndef _GRAPHICS_DRAWING_H_
#define _GRAPHICS_DRAWING_H_

#include "graphics/color.h"

#include <cglm/types-struct.h>
#include <stdint.h>

extern void draw_quad(vec2s pos, vec2s dim, uint32_t color);
extern void draw_line(vec2s start, vec2s end, uint32_t color);
extern void draw_text(vec2s pos, float scale, uint32_t color,
                      const char *format, ...);

#endif
