#ifndef _GRAPHICS_VAO_H_
#define _GRAPHICS_VAO_H_

#include <glad/glad.h>

/* Creates an vertex array. */
GLuint vao_create();

/* Releases an vertex array. */
void vao_destroy(GLuint *);

/* Binds a vertex array. */
void vao_bind(GLuint *);

/* Unbinds an vertex array. */
void vao_unbind();

/* Sets the vertex attributes for `data`. */
void vao_attrib(GLint index, GLint size, GLenum type, GLsizei stride,
                const void *data);

#endif // ifndef _GRAPHICS_VAO_H_
