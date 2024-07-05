#ifndef _GRAPHICS_RENDERER_H_
#define _GRAPHICS_RENDERER_H_

#include "game/camera.h"
#include "graphics/shader.h"

#include <glad/glad.h>

typedef enum {
    SHADER_DEFAULT,
    SHADER_UI_TEXT,
    SHADER_UI_TEXTURE,
    SHADER_MAX,
} shader_e;

typedef struct {
    shader_t shaders[SHADER_MAX];
    shader_t *shader;
    const camera_t *camera;
} renderer_t;

void renderer_init();
void renderer_destroy();

shader_t *renderer_use_shader(shader_e);

void renderer_prepare_scene(const camera_t *);
void renderer_present_scene();

#endif // _GRAPHICS_RENDERER_H_
