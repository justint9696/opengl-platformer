#ifndef _GRAPHICS_IBO_H_
#define _GRAPHICS_IBO_H_

#include <glad/glad.h>
#include <stddef.h>

/* Creates an index buffer and sets its data. */
GLuint ibo_create(const void *data, size_t len);

/* Releases an index buffer. */
void ibo_destroy(GLuint *);

/* Binds an index buffer. */
void ibo_bind(GLuint *);

/* Unbinds an index buffer. */
void ibo_unbind();

/* Sets the data for an index buffer. */
void ibo_buffer_data(const void *data, size_t len);

#endif // ifndef _GRAPHICS_IBO_H_
