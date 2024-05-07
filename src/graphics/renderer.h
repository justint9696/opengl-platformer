#pragma once

#include "game/camera.h"
#include "graphics/shader.h"

#include <glad/glad.h>

typedef struct { 
    GLuint vao, vbo, ibo;
    shader_s shader;
    const camera_s *camera;
} renderer_s;

void renderer_init();
void renderer_destroy();

void renderer_prepare_scene(const camera_s *);
void renderer_present_scene();
