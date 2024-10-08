/**
 * @file shader.c
 * @author Justin Tonkinson
 * @date 2024/05/06
 * @brief Shader implementation functions.
 */

#include "graphics/shader.h"
#include "util/log.h"

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
    glShaderSource(handle, 1, (const GLchar *const *)&text,
                   (const GLint *)&size);
    glCompileShader(handle);

    GLint result;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &result);

    if (!result) {
        char log[512];
        glGetShaderInfoLog(handle, 512, NULL, log);
        log_fatal("%s\n%s\n", fpath, log);
    }

    free(text);
    return handle;
}

GLint shader_create(const char *vs_path, const char *fs_path) {
    GLint vs = shader_compile(vs_path, GL_VERTEX_SHADER);
    GLint fs = shader_compile(fs_path, GL_FRAGMENT_SHADER);
    GLint shader = glCreateProgram();

    glAttachShader(shader, vs);
    glAttachShader(shader, fs);
    glLinkProgram(shader);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return shader;
}

void shader_destroy(GLint self) {
    glDeleteProgram(self);
}

void shader_use(GLint self) {
    glUseProgram(self);
}

void shader_uniform_mat4f(GLint self, const char *uniform, mat4s val) {
    glUniformMatrix4fv(glGetUniformLocation(self, uniform), 1, GL_FALSE,
                       (const GLfloat *)&val);
}

void shader_uniform_vec3f(GLint self, const char *uniform, vec3s val) {
    glUniform3fv(glGetUniformLocation(self, uniform), 1, (const GLfloat *)&val);
}

void shader_uniform_vec4f(GLint self, const char *uniform, vec4s val) {
    glUniform4fv(glGetUniformLocation(self, uniform), 1, (const GLfloat *)&val);
}

void shader_uniform_3f(GLint self, const char *uniform, float x, float y,
                       float z) {
    glUniform3f(glGetUniformLocation(self, uniform), x, y, z);
}
