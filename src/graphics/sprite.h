/**
 * @file sprite.h
 * @author Justin Tonkinson
 * @date 2024/10/08
 * @brief This file contains the data structures and function prototypes needed
 * for sprite batching.
 */

#ifndef _GRAPHICS_SPRITE_H_
#define _GRAPHICS_SPRITE_H_

#include "game/camera.h"

#include <cglm/types-struct.h>
#include <glad/glad.h>
#include <stdint.h>

/** @brief Sprite data structure. */
typedef struct sprite_s {
    /** @brief The world position of the sprite. */
    vec2s pos;

    /** @brief The size of the sprite. */
    vec2s dim;

    /** @brief The Z position of the sprite. */
    float z;

    /** @brief The color (RGBA) of the sprite. */
    uint32_t color;
} sprite_t;

/** @brief Sprite batch data structure. */
typedef struct sprite_batch_s {
    /** @brief Dynamic array of sprites to be rendered. */
    sprite_t *arr;

    /** @brief Vertex buffer objects. */
    GLuint vao, vbo, ibo;
} sprite_batch_t;

/**
 * @brief Initializes the sprite batch internal state.
 * @param self a pointer to a sprite batch
 */
void sprites_init();

/**
 * @brief Destroys the sprite batch internal state.
 * @param self a pointer to a sprite batch
 */
void sprites_destroy();

/**
 * @brief Creats a sprite and adds it to the batch.
 * @param self a pointer to a sprite batch
 * @param pos position of the sprite
 * @param dim dimensions of the sprite
 * @param z z position of the sprite
 * @param color color of the sprite
 */
void sprites_push(vec2s pos, vec2s dim, float z, uint32_t color);

/**
 * @brief Renders the sprite batch and clears its contents.
 * @param self a pointer to a sprite batch
 * @param camera a pointer to the scene camera
 */
void sprites_render(const camera_t *);

#endif // ifndef _GRAPHICS_SPRITE_H_
