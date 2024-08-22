#ifndef _GRAPHICS_TEXTURE_H_
#define _GRAPHICS_TEXTURE_H_

#include <glad/glad.h>
#include <stdint.h>

typedef struct {
    GLuint handle;
    int32_t width, height;
    int32_t channels;
} texture_t;

texture_t texture_create(const char *fname);
void texture_destroy(texture_t);

void texture_bind(texture_t);

#endif // ifndef _GRAPHICS_TEXTURE_H_
