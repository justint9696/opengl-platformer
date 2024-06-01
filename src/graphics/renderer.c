#include "graphics/renderer.h"

#include "graphics/drawing.h"
#include "graphics/window.h"

#include <cglm/struct.h>
#include <string.h>

static renderer_t renderer;

void renderer_init() {
    memset(&renderer, 0, sizeof(renderer_t));

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

void renderer_prepare_scene(const camera_t *camera) {
    renderer.camera = camera;
    glClear(GL_COLOR_BUFFER_BIT);
}

void renderer_present_scene() {
    glUseProgram(renderer.shader.handle);
    glUniformMatrix4fv(glGetUniformLocation(renderer.shader.handle, "projection"), 1, GL_FALSE,
            (const GLfloat *)&renderer.camera->projection);
    glUniformMatrix4fv(glGetUniformLocation(renderer.shader.handle, "view"), 1, GL_FALSE,
            (const GLfloat *)&renderer.camera->view);
}

void draw_quad(vec2s pos, vec2s dim, uint32_t color) {
    float vertices[] = {
        0.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        1.f, 1.f, 0.f,
        1.f, 0.f, 0.f,
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    mat4s model = glms_mat4_identity();
    model = glms_translate(model, (vec3s) { pos.x, pos.y, 0.f });
    model = glms_scale(model, (vec3s) { dim.x, dim.y, 1.f });

    glBindVertexArray(renderer.vao);

    glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glUniform4fv(glGetUniformLocation(renderer.shader.handle, "color"), 1,
                 (const GLfloat *)&RGBA(color));

    glUniformMatrix4fv(glGetUniformLocation(renderer.shader.handle, "model"), 1,
                       GL_FALSE, (const GLfloat *)&model);

    glUniform4fv(glGetUniformLocation(renderer.shader.handle, "color"), 1,
                 (const GLfloat *)&RGBA(color));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void draw_line(vec2s start, vec2s end, uint32_t color) {
    float x = (end.x - start.x), y = (end.y - start.y);
    float vertices[] = {
        0.f, 0.f, 0.f,
        x,   y, 0.f,
    };

    mat4s model = glms_mat4_identity();
    model = glms_translate(model, (vec3s) { start.x, start.y, 0.f });

    glBindVertexArray(renderer.vao);

    glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    glUniformMatrix4fv(glGetUniformLocation(renderer.shader.handle, "model"), 1, GL_FALSE,
            (const GLfloat *)&model);

    glUniform4fv(glGetUniformLocation(renderer.shader.handle, "color"), 1,
            (const GLfloat *)&RGBA(color));

    glDrawArrays(GL_LINES, 0, 2);
}
