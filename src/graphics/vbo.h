#ifndef _GRAPHICS_VBO_H_
#define _GRAPHICS_VBO_H_

#include <glad/glad.h>
#include <stddef.h>

/* Creates a vertex buffer and sets its data. */
GLuint vbo_create(const void *data, size_t len);

/* Releases a vertex buffer. */
void vbo_destroy(GLuint *);

/* Binds a vertex buffer. */
void vbo_bind(GLuint *);

/* Unbinds a vertex buffer. */
void vbo_unbind();

/* Sets the data for a vertex buffer. */
void vbo_buffer_data(const void *data, size_t len);

#endif // ifndef _GRAPHICS_VBO_H_
