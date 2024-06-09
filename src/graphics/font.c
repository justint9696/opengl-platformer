#include "graphics/font.h"

#include "util/io.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <inttypes.h>

fontchar_t FONT_TABLE[FONTCHAR_MAX];

static void build_font_table(FT_Face face) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (uint8_t c = 0; c < FONTCHAR_MAX; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            WARN(0, "Failed to load Glyph.\n");
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
    ASSERT(!FT_Init_FreeType(&ft), "Could not initialize FreeType Library\n");

    FT_Face face;
    ASSERT(!FT_New_Face(ft, fpath, 0, &face), "Failed to load font: %s\n",
           fpath);

    FT_Set_Pixel_Sizes(face, 0, 24);

    build_font_table(face);

    // clear resources
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}
