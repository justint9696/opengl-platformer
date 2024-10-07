/**
 * @file texture.h
 * @author Justin Tonkinson
 * @date 2024/08/22
 * @brief OpenGL texture wrapper function prototypes.
 */

#ifndef _GRAPHICS_TEXTURE_H_
#define _GRAPHICS_TEXTURE_H_

#include <glad/glad.h>
#include <stdint.h>

typedef struct {
    GLuint handle;
    int32_t width, height;
    int32_t channels;
} texture_t;

/**
 * @brief Creates a texture from the file and returns it.
 * @param fpath texture file relative path
 * @returns the resultant texture
 */
texture_t texture_create(const char *fpath);

/**
 * @brief Destroys a texture.
 * @param self the texture to destroy
 */
void texture_destroy(texture_t);

/**
 * @brief Binds a texture.
 * @param self the texture to bind
 */
void texture_bind(texture_t);

#endif // ifndef _GRAPHICS_TEXTURE_H_
