#ifndef _GRAPHICS_SHADER_H_
#define _GRAPHICS_SHADER_H_

#include <glad/glad.h>

typedef struct {
    GLuint handle, fs, vs;
} shader_t;

shader_t shader_create(const char *vs_path, const char *fs_path);
void shader_destroy(shader_t);

#endif
