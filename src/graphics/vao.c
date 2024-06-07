#include "graphics/vao.h"

GLuint vao_create() {
    GLuint self;
    glGenVertexArrays(1, &self);
    return self;
}

void vao_destroy(GLuint *self) {
    glDeleteVertexArrays(1, self);
}

void vao_bind(GLuint *self) {
    glBindVertexArray(*self);
}

void vao_attrib(GLint index, GLint size, GLenum type, GLsizei stride,
                const void *data) {
    glVertexAttribPointer(index, size, type, GL_FALSE, stride, data);
    glEnableVertexAttribArray(index);
}
