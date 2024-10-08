/**
 * @file vbo.c
 * @author Justin Tonkinson
 * @date 2024/06/07
 * @brief OpenGL vertex buffer wrapper functions.
 */

#include "graphics/vbo.h"

inline GLuint vbo_create(const void *data, GLsizei len) {
    GLuint self;
    glGenBuffers(1, &self);
    glBindBuffer(GL_ARRAY_BUFFER, self);
    glBufferData(GL_ARRAY_BUFFER, len, data, GL_DYNAMIC_DRAW);
    return self;
}

inline void vbo_destroy(GLuint *self) {
    glDeleteBuffers(1, self);
}

inline void vbo_bind(GLuint *self) {
    glBindBuffer(GL_ARRAY_BUFFER, *self);
}

inline void vbo_unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline void vbo_buffer_data(const void *data, GLsizei len) {
    glBufferData(GL_ARRAY_BUFFER, len, data, GL_DYNAMIC_DRAW);
}

inline void vbo_buffer_sub_data(const void *data, GLsizei offset, GLsizei len) {
    glBufferSubData(GL_ARRAY_BUFFER, offset, len, data);
}
