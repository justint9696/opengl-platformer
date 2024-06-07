#ifndef _RENDERER_FONT_H_
#define _RENDERER_FONT_H_

#include "graphics/shader.h"

#include <cglm/types-struct.h>

#define FONTCHAR_MAX 128

typedef struct fontchar_s {
    // texture id
    GLuint id;

    // size of texture
    ivec2s dim;

    // offset from base to left/top of texture
    ivec2s bearing;

    // offset to next texture
    GLuint advance;
} fontchar_t;

void font_init(const char *fpath);

extern fontchar_t FONT_TABLE[FONTCHAR_MAX];

#endif
