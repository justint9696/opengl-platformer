/**
 * @file font.h
 * @author Justin Tonkinson
 * @date 2024/06/07
 * @brief Font data structures and function prototypes.
 */

#ifndef _GRAPHICS_FONT_H_
#define _GRAPHICS_FONT_H_

#include <cglm/types-struct.h>
#include <glad/glad.h>
#include <stddef.h>

#define FONTCHAR_MAX 128

typedef struct fontchar_s {
    /** @brief Texture handle. */
    GLuint id;

    /** @brief Dimensions of the texture. */
    ivec2s dim;

    /** @brief Offset from the base to the top left of the texture. */
    ivec2s bearing;

    /** @brief Offset of the next texture. */
    GLuint advance;
} fontchar_t;

/**
 * @brief Initializes the internal font table.
 * @param relative path of font file
 */
void font_init(const char *fpath);

/**
 * @brief Releases the allocated memory for the internal font table.
 */
void font_destroy();

/**
 * @brief Returns a fontchar from the ASCII value of a character.
 * @param idx index of the char
 */
const fontchar_t *font_get_char(size_t idx);

#endif // ifndef _GRAPHICS_FONT_H_
