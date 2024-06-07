#ifndef _GRAPHICS_RENDERER_H_
#define _GRAPHICS_RENDERER_H_

#include "game/camera.h"
#include "graphics/shader.h"

#include <glad/glad.h>

typedef enum {
    SHADER_DEFAULT,
    SHADER_TEXT,
    SHADER_UI,
    SHADER_MAX,
} shadertype_t;

typedef struct {
    shader_t shaders[SHADER_MAX];
    shader_t *shader;
    const camera_t *camera;
} renderer_t;

void renderer_init();
void renderer_destroy();

void renderer_use_shader(shadertype_t);

void renderer_prepare_scene(const camera_t *);
void renderer_present_scene();

extern const float QUAD_VERTICES[12];
extern const unsigned int QUAD_INDICES[6];

#endif
