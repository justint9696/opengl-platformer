/**
 * @file texture.c
 * @author Justin Tonkinson
 * @date 2024/08/22
 * @brief Texture implementation functions.
 */

#include "graphics/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <assert.h>
#include <stdbool.h>

texture_t texture_create(const char *fpath) {
    texture_t self;

    glGenTextures(1, &self.handle);
    glBindTexture(GL_TEXTURE_2D, self.handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data
        = stbi_load(fpath, &self.width, &self.height, &self.channels, 0);
    assert(data);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, self.width, self.height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    return self;
}

void texture_destroy(texture_t self) {
    glDeleteTextures(1, &self.handle);
}

void texture_bind(texture_t self) {
    glBindTexture(GL_TEXTURE_2D, self.handle);
}
