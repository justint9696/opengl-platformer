#include "graphics/ibo.h"

GLuint ibo_create(const void *data, size_t len) {
    GLuint self;
    glGenBuffers(1, &self);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, len, data, GL_DYNAMIC_DRAW);
    return self;
}

void ibo_destroy(GLuint *self) {
    glDeleteBuffers(1, self);
}

void ibo_bind(GLuint *self) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *self);
}

void ibo_buffer_data(const void *data, size_t len) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, len, data, GL_DYNAMIC_DRAW);
}
