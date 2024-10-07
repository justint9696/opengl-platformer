/**
 * @file vao.h
 * @author Justin Tonkinson
 * @date 2024/06/07
 * @brief OpenGL vertex array wrapper implementation functions.
 */

#include "graphics/vao.h"

inline GLuint vao_create() {
    GLuint self;
    glGenVertexArrays(1, &self);
    return self;
}

inline void vao_destroy(GLuint *self) {
    glDeleteVertexArrays(1, self);
}

inline void vao_bind(GLuint *self) {
    glBindVertexArray(*self);
}

inline void vao_unbind() {
    glBindVertexArray(0);
}

inline void vao_attrib(GLint index, GLint size, GLenum type, GLsizei stride,
                       const void *offset) {
    glVertexAttribPointer(index, size, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(index);
}
