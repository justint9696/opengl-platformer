#include "graphics/vbo.h"

inline GLuint vbo_create(const void *data, size_t len) {
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

inline void vbo_buffer_data(const void *data, size_t len) {
    glBufferData(GL_ARRAY_BUFFER, len, data, GL_DYNAMIC_DRAW);
}
