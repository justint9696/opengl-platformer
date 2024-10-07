/**
 * @file font.c
 * @author Justin Tonkinson
 * @date 2024/06/07
 * @brief Font implementation functions.
 */

#include "graphics/font.h"

#include "util/assert.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <assert.h>
#include <stdint.h>

static fontchar_t FONT_TABLE[FONTCHAR_MAX];

static void build_font_table(FT_Face face) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (uint8_t c = 0; c < FONTCHAR_MAX; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            log_warn("Failed to load Glyph `%c`.\n", c);
        }

        // generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                     face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        FONT_TABLE[c] = (fontchar_t) {
            texture,
            (ivec2s) { face->glyph->bitmap.width, face->glyph->bitmap.rows },
            (ivec2s) {  face->glyph->bitmap_left,  face->glyph->bitmap_top },
            face->glyph->advance.x,
        };
    }
}

void font_init(const char *fpath) {
    FT_Library ft;
    XASSERT(FT_Init_FreeType(&ft) == 0,
            "Could not initialize FreeType Library\n");

    FT_Face face;
    XASSERT(FT_New_Face(ft, fpath, 0, &face) == 0, "Failed to load font: %s\n",
            fpath);

    FT_Set_Pixel_Sizes(face, 0, 18);

    build_font_table(face);

    // clear resources
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void font_destroy() {
    for (size_t i = 0; i < FONTCHAR_MAX; i++) {
        fontchar_t *fontchar = &FONT_TABLE[i];
        glDeleteTextures(1, &fontchar->id);
    }
}

const fontchar_t *font_get_char(size_t idx) {
    assert(idx < 128);
    return &FONT_TABLE[idx];
}
