/**
 * @file shader.h
 * @author Justin Tonkinson
 * @date 2024/05/06
 * @brief OpenGL shader wrapper function prototypes.
 */

#ifndef _GRAPHICS_SHADER_H_
#define _GRAPHICS_SHADER_H_

#include <cglm/types-struct.h>
#include <glad/glad.h>

typedef struct {
    GLuint handle, fs, vs;
} shader_t;

/**
 * @brief Creates a shader program from the provided files.
 * @param vs_path vertex shader relative path
 * @param fs_path fragment shader relative path
 * @returns the resultant shader program
 */
shader_t shader_create(const char *vs_path, const char *fs_path);

/**
 * @brief Releases a shader program.
 * @param self the shader to destroy
 */
void shader_destroy(shader_t);

/**
 * @brief Binds a shader program.
 * @param self the shader to use
 */
void shader_use(shader_t);

/**
 * @brief Sets a shader's 4d float matrix uniform.
 * @param self the shader to reference
 * @param uniform the shader uniform to set
 * @param val the uniform value
 */
void shader_uniform_mat4f(shader_t, const char *uniform, mat4s val);

/**
 * @brief Sets a shader's 3d float vector uniform.
 * @param self the shader to reference
 * @param uniform the shader uniform to set
 * @param val the uniform value
 */
void shader_uniform_vec3f(shader_t, const char *uniform, vec3s val);

/**
 * @brief Sets a shader's 4d float vector uniform.
 * @param self the shader to reference
 * @param uniform the shader uniform to set
 * @param val the uniform value
 */
void shader_uniform_vec4f(shader_t, const char *uniform, vec4s val);

/**
 * @brief Sets a shader's 3 float uniform.
 * @param self the shader to reference
 * @param uniform the shader uniform to set
 * @param x uniform x value
 * @param y uniform y value
 * @param z uniform z value
 */
void shader_uniform_3f(shader_t, const char *uniform, float x, float y, float z);

#endif // ifndef _GRAPHICS_SHADER_H_
