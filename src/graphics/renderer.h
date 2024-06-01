#ifndef _GRAPHICS_RENDERER_H_
#define _GRAPHICS_RENDERER_H_

#include "game/camera.h"
#include "graphics/shader.h"

#include <glad/glad.h>

typedef struct {
    GLuint vao, vbo, ibo;
    shader_t shader;
    const camera_t *camera;
} renderer_t;

void renderer_init();
void renderer_destroy();

void renderer_prepare_scene(const camera_t *);
void renderer_present_scene();

#endif
