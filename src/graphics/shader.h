#ifndef _GRAPHICS_SHADER_H_
#define _GRAPHICS_SHADER_H_

#include <cglm/types-struct.h>
#include <glad/glad.h>

typedef struct {
    GLuint handle, fs, vs;
} shader_t;

shader_t shader_create(const char *vs_path, const char *fs_path);
void shader_destroy(shader_t);

void shader_use(shader_t);
void shader_uniform_mat4f(shader_t, const char *uniform, mat4s val);
void shader_uniform_vec3f(shader_t, const char *uniform, vec3s val);
void shader_uniform_vec4f(shader_t, const char *uniform, vec4s val);
void shader_uniform_3f(shader_t, const char *uniform, float x, float y, float z);

#endif // _GRAPHICS_SHADER_H_
