/**
 * @file renderer.h
 * @author Justin Tonkinson
 * @date 2024/05/06
 * @brief Renderer data structure and function prototypes.
 */

#ifndef _GRAPHICS_RENDERER_H_
#define _GRAPHICS_RENDERER_H_

#include "game/camera.h"
#include "graphics/shader.h"
#include "graphics/sprite.h"
#include "graphics/texture.h"

#include <glad/glad.h>

/** @brief All possible shader types. */
typedef enum shader_type_e {
    SHADER_DEFAULT,
    SHADER_UI_TEXT,
    SHADER_UI_TEXTURE,
    SHADER_MAX,
} shader_type_t;

/** @brief Renderer data structure. */
typedef struct {
    /** @brief An array of the shaders. */
    GLint shaders[SHADER_MAX];

    /** @brief The currently bound shader. */
    GLint shader;

    sprite_batch_t sprites;

    /** @brief */
    /* texture_t texture; */

    /** @brief A pointer to the camera the scene should be rendered from. */
    const camera_t *camera;
} renderer_t;

/**
 * @brief Initializes the internal renderer.
 */
void renderer_init();

/**
 * @brief Destroys the internal renderer.
 */
void renderer_destroy();

/**
 * @brief Binds the provided shader index if it has not already been bound.
 * @param index index of the shader to bind
 */
GLint renderer_use_shader(shader_type_t index);

/**
 * @brief Prepares the renderer scene based on the provided camera.
 * @brief camera a pointer to the scene camera
 */
void renderer_prepare_scene(const camera_t *camera);

/**
 * @brief Presents the renderer scene.
 */
void renderer_present_scene();

#endif // ifndef _GRAPHICS_RENDERER_H_
