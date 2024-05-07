#include "graphics/renderer.h"
#include "graphics/color.h"
#include "graphics/drawing.h"
#include "graphics/window.h"

#include <string.h>

static renderer_s renderer;

void renderer_init() {
    memset(&renderer, 0, sizeof(renderer_s));

    glGenVertexArrays(1, &renderer.vao);
    glGenBuffers(1, &renderer.vbo);
    glGenBuffers(1, &renderer.ibo);

    glClearColor(0.53f, 0.81f, 0.92f, 1.f);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // load shaders
    renderer.shader = shader_create("shaders/basic2d.vs", "shaders/basic2d.fs");
}

void renderer_destroy() {
    glDeleteVertexArrays(1, &renderer.vao);
    glDeleteBuffers(1, &renderer.vbo);
    glDeleteBuffers(1, &renderer.ibo);
    shader_destroy(renderer.shader);
}

void renderer_prepare_scene() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void renderer_present_scene() {
    glUseProgram(renderer.shader.handle);
}

void draw_quad(vec2s pos, vec2s dim, uint32_t color) {
    float vertices[] = {
        -0.5f, -0.5f, 0.f,
        -0.5f, +0.5f, 0.f,
        +0.5f, +0.5f, 0.f,
        +0.5f, -0.5f, 0.f,
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glBindVertexArray(renderer.vao);

    glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

     glUniform4fv(glGetUniformLocation(renderer.shader.handle, "color"), 1,
            (const GLfloat *)&RGBA(color));

     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
