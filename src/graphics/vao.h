#ifndef _GRAPHICS_VAO_H_
#define _GRAPHICS_VAO_H_

#include <glad/glad.h>

GLuint vao_create();
void vao_destroy(GLuint *);
void vao_bind(GLuint *);

void vao_attrib(GLint index, GLint size, GLenum type, GLsizei stride,
                const void *data);

#endif
