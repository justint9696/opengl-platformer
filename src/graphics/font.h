#ifndef _GRAPHICS_FONT_H_
#define _GRAPHICS_FONT_H_

#include <cglm/types-struct.h>
#include <glad/glad.h>
#include <stddef.h>

#define FONTCHAR_MAX 128

typedef struct fontchar_s {
    // texture id
    GLuint id;

    // size of texture
    ivec2s dim;

    // offset from the base to the left/top of texture
    ivec2s bearing;

    // offset of next texture
    GLuint advance;
} fontchar_t;

void font_init(const char *fpath);

// returns a fontchar from the ASCII value (or `idx`) of a character
const fontchar_t *font_get_char(size_t idx);

#endif // _GRAPHICS_FONT_H_
