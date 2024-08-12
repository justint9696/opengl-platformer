#ifndef _GRAPHICS_COLOR_H_
#define _GRAPHICS_COLOR_H_

#include <cglm/types-struct.h>

#define COLOR_WHITE  0xFFFFFFFF
#define COLOR_BLACK  0x000000FF
#define COLOR_RED    0xFF0000FF
#define COLOR_GREEN  0x00FF00FF
#define COLOR_BLUE   0x0000FFFF
#define COLOR_CYAN   0x00FFFFFF
#define COLOR_ORANGE 0xFFA500FF
#define COLOR_YELLOW 0xFFFF00FF

/* Converts a color represented as an integer (RGBA) into a 4d float vector. */
#define RGBA(c) ((vec4s) {\
        ((c >> 24) & 0xFF) / 255.f,\
        ((c >> 16) & 0xFF) / 255.f,\
        ((c >>  8) & 0xFF) / 255.f,\
        ((c >>  0) & 0xFF) / 255.f,\
    })

#endif // ifndef _GRAPHICS_COLOR_H_
