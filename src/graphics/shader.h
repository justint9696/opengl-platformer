#ifndef _GRAPHICS_SHADER_H_
#define _GRAPHICS_SHADER_H_

#include <cglm/types-struct.h>
#include <glad/glad.h>

typedef struct {
    GLuint handle, fs, vs;
} shader_t;

/* Creates a shader program from the provided files. */
shader_t shader_create(const char *vs_path, const char *fs_path);

/* Releases a shader program. */
void shader_destroy(shader_t);

/* Binds a shader program. */
void shader_use(shader_t);

/* Sets a shader's 4d float matrix uniform. */
void shader_uniform_mat4f(shader_t, const char *uniform, mat4s val);

/* Sets a shader's 3d float vector uniform. */
void shader_uniform_vec3f(shader_t, const char *uniform, vec3s val);

/* Sets a shader's 4d float vector uniform. */
void shader_uniform_vec4f(shader_t, const char *uniform, vec4s val);

/* Sets a shader's 3 float uniform. */
void shader_uniform_3f(shader_t, const char *uniform, float x, float y, float z);

#endif // ifndef _GRAPHICS_SHADER_H_
