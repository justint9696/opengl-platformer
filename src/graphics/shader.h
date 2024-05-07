#pragma once

#include <glad/glad.h>

typedef struct {
    GLuint handle, fs, vs;
} shader_s;

shader_s shader_create(const char *vs_path, const char *fs_path);
void shader_destroy(shader_s);
