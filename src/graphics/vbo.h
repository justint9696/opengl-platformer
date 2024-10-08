/**
 * @file vbo.h
 * @author Justin Tonkinson
 * @date 2024/06/07
 * @brief OpenGL vertex buffer wrapper functions.
 */

#ifndef _GRAPHICS_VBO_H_
#define _GRAPHICS_VBO_H_

#include <glad/glad.h>
#include <stddef.h>

/**
 * @brief Creates a vertex buffer and sets its data.
 * @param data data to copy to the buffer
 * @param len size of the data in bytes
 */
GLuint vbo_create(const void *data, GLsizei len);

/**
 * @brief Releases a vertex buffer.
 * @param self vertex buffer handle
 */
void vbo_destroy(GLuint *self);

/**
 * @brief Binds a vertex buffer.
 * @param self vertex buffer handle
 */
void vbo_bind(GLuint *self);

/**
 * @brief Unbinds a vertex buffer.
 */
void vbo_unbind();

/**
 * @brief Sets the data for a vertex buffer.
 * @param data data to copy to the buffer
 * @param len size of the data in bytes
 */
void vbo_buffer_data(const void *data, GLsizei len);

/**
 * @brief Sets the sub data for a vertex buffer.
 * @param data data to copy to the buffer
 * @param offset offset of the data
 * @param len size of the data in bytes
 */
void vbo_buffer_sub_data(const void *data, GLsizei offset, GLsizei len);

#endif // ifndef _GRAPHICS_VBO_H_
