#pragma once

#include "graphics/shader.h"

#include <glad/glad.h>

typedef struct { 
    GLuint vao, vbo, ibo;
    shader_s shader;
} renderer_s;

void renderer_init();
void renderer_destroy();

void renderer_prepare_scene();
void renderer_present_scene();
