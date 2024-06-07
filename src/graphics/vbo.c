#include "graphics/vbo.h"

GLuint vbo_create(const void *data, size_t len) {
    GLuint self;
    glGenBuffers(1, &self);
    glBindBuffer(GL_ARRAY_BUFFER, self);
    glBufferData(GL_ARRAY_BUFFER, len, data, GL_DYNAMIC_DRAW);
    return self;
}

void vbo_destroy(GLuint *self) {
    glDeleteBuffers(1, self);
}

void vbo_bind(GLuint *self) {
    glBindBuffer(GL_ARRAY_BUFFER, *self);
}

void vbo_buffer_data(GLuint *self, const void *data, size_t len) {
    glBindBuffer(GL_ARRAY_BUFFER, *self);
    glBufferData(GL_ARRAY_BUFFER, len, data, GL_DYNAMIC_DRAW);
}
