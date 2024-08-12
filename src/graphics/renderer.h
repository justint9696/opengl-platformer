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

/* Initializes the internal renderer. */
void renderer_init();

/* Destroys the internal renderer. */
void renderer_destroy();

/* Binds the provided shader index if it has not already been bound. */
shader_t *renderer_use_shader(shader_e);

/* Prepares the renderer scene based on the provided camera. */
void renderer_prepare_scene(const camera_t *);

/* Presents the renderer scene. */
void renderer_present_scene();

#endif // ifndef _GRAPHICS_RENDERER_H_
