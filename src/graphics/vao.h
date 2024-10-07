/**
 * @file vao.h
 * @author Justin Tonkinson
 * @date 2024/06/07
 * @brief OpenGL vertex array wrapper function prototypes.
 */

#ifndef _GRAPHICS_VAO_H_
#define _GRAPHICS_VAO_H_

#include <glad/glad.h>

/**
 * @brief Creates a vertex array.
 */
GLuint vao_create();

/**
 * @brief Releases a vertex array.
 * @param self vertex array handle
 */
void vao_destroy(GLuint *self);

/**
 * @brief Binds a vertex array.
 * @param self vertex array handle
 */
void vao_bind(GLuint *self);

/**
 * @brief Unbinds a vertex array.
 */
void vao_unbind();

/**
 * @brief Sets the vertex attributes for data.
 * @param index index of the vertex attribute
 * @param size number of components per vertex attribute
 * @param type data type of the vertex attribute
 * @param stride byte offset of the vertex attribute
 * @param pointer offset of the first component of the vertex attribute
 */
void vao_attrib(GLint index, GLint size, GLenum type, GLsizei stride,
                const void *offset);

#endif // ifndef _GRAPHICS_VAO_H_
