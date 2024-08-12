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

/* Initializes the internal font table. */
void font_init(const char *fpath);

/* Releases the allocated memory for the internal font table. */
void font_destroy();

/* Returns a fontchar from the ASCII value (or `idx`) of a character. */
const fontchar_t *font_get_char(size_t idx);

#endif // ifndef _GRAPHICS_FONT_H_
