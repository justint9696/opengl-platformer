#ifndef _GRAPHICS_VBO_H_
#define _GRAPHICS_VBO_H_

#include <glad/glad.h>
#include <stddef.h>

GLuint vbo_create(const void *data, size_t len);
void vbo_destroy(GLuint *);
void vbo_bind(GLuint *);
void vbo_buffer_data(const void *data, size_t len);

#endif
