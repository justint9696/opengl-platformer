#include "graphics/shader.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static GLint shader_compile(const char *fpath, GLenum type) {
    FILE *fp = fopen(fpath, "rb");
    assert(fp);

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    assert(size);

    char *text = calloc(1, size);
    fseek(fp, 0, SEEK_SET);
    fread(text, 1, size, fp);
    assert(strlen(text));

    fclose(fp);

    GLuint handle = glCreateShader(type);
    glShaderSource(handle, 1, (const GLchar *const *)&text, (const GLint *)&size);
    glCompileShader(handle);

    GLint result;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &result);

    if (!result) {
        char log[512];
        glGetShaderInfoLog(handle, 512, NULL, log);
        fprintf(stderr, "ERROR: %s\n%s\n", fpath, log);
        exit(1);
    }

    free(text);
    return handle;
}

shader_s shader_create(const char *vs_path, const char *fs_path) {
    shader_s self;
    self.vs = shader_compile(vs_path, GL_VERTEX_SHADER);
    self.fs = shader_compile(fs_path, GL_FRAGMENT_SHADER);
    self.handle = glCreateProgram();

    glAttachShader(self.handle, self.vs);
    glAttachShader(self.handle, self.fs);
    glLinkProgram(self.handle);

    return self;
}

void shader_destroy(shader_s self) {
    glDeleteProgram(self.handle);
    glDeleteShader(self.vs);
    glDeleteShader(self.fs);
}
