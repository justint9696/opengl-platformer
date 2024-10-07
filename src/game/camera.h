/**
 * @brief camera.h
 * @author Justin Tonkinson
 * @date 2024/05/07
 * @brief Camera function prototypes.
 */

#ifndef _GAME_CAMERA_H_
#define _GAME_CAMERA_H_

#include <cglm/types-struct.h>

/** @brief General camera data structure. */
typedef struct {
    /** @brief Camera position in world space. */
    vec3s pos;

    /** @brief Camera projection matrix. */
    mat4s projection;

    /** @brief Camera view matrix. */
    mat4s view;
} camera_t;

/**
 * @brief Initializes a camera with a frustrum of size (width x height).
 * @param self a pointer to a camera
 * @param pos starting position in 2-dimensional space
 * @param width the width of the frustrum
 * @param height the height of the frustrum
 */
void camera_init(camera_t *, vec2s pos, int width, int height);

/**
 * @brief Updates the camera's view matrix.
 * @param self a pointer to a camera
 */
void camera_update_view(camera_t *);

#endif // ifndef _GAME_CAMERA_H_
