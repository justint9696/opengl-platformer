#ifndef _GRAPHICS_IBO_H_
#define _GRAPHICS_IBO_H_

#include <glad/glad.h>
#include <stddef.h>

GLuint ibo_create(const void *data, size_t size);
void ibo_destroy(GLuint *);
void ibo_bind(GLuint *);
void ibo_buffer_data(GLuint *, const void *data, size_t size);

#endif
