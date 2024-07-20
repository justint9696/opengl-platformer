#ifndef _GRAPHICS_VAO_H_
#define _GRAPHICS_VAO_H_

#include <glad/glad.h>

GLuint vao_create();
void vao_destroy(GLuint *);
void vao_bind(GLuint *);

// sets the vertex attributes of `data`
void vao_attrib(GLint index, GLint size, GLenum type, GLsizei stride,
                const void *data);

#endif // _GRAPHICS_VAO_H_
