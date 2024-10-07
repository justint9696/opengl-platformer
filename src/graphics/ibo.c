/**
 * @file ibo.c
 * @author Justin Tonkinson
 * @date 2024/06/07
 * @brief OpenGL index buffer wrapper implementation functions.
 */

#include "graphics/ibo.h"

inline GLuint ibo_create(const void *data, size_t len) {
    GLuint self;
    glGenBuffers(1, &self);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, len, data, GL_DYNAMIC_DRAW);
    return self;
}

inline void ibo_destroy(GLuint *self) {
    glDeleteBuffers(1, self);
}

inline void ibo_bind(GLuint *self) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *self);
}

inline void ibo_unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

inline void ibo_buffer_data(const void *data, size_t len) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, len, data, GL_DYNAMIC_DRAW);
}
