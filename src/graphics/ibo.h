/**
 * @file ibo.h
 * @author Justin Tonkinson
 * @date 2024/06/07
 * @brief OpenGL index buffer wrapper function prototypes.
 */

#ifndef _GRAPHICS_IBO_H_
#define _GRAPHICS_IBO_H_

#include <glad/glad.h>
#include <stddef.h>

/**
 * @brief Creates an index buffer and sets its data.
 * @param data data to copy to the buffer
 * @param len size of the data in bytes
 */
GLuint ibo_create(const void *data, GLsizei len);

/**
 * @brief Releases an index buffer.
 * @param self index buffer handle
 */
void ibo_destroy(GLuint *self);

/**
 * @brief Binds an index buffer.
 * @param self index buffer handle
 */
void ibo_bind(GLuint *self);

/**
 * @brief Unbinds an index buffer.
 */
void ibo_unbind();

/**
 * @brief Sets the data for an index buffer.
 * @param data data to copy to the buffer
 * @param len size of the data in bytes
 */
void ibo_buffer_data(const void *data, GLsizei len);

/**
 * @brief Sets the sub data for an index buffer.
 * @param data data to copy to the buffer
 * @param offset offset of the data
 * @param len size of the data in bytes
 */
void ibo_buffer_sub_data(const void *data, GLsizei offset, GLsizei len);

#endif // ifndef _GRAPHICS_IBO_H_
