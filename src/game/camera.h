#ifndef _GAME_CAMERA_H_
#define _GAME_CAMERA_H_

#include <cglm/types-struct.h>

typedef struct {
    vec3s pos;
    mat4s projection, view;
} camera_t;

/* Initializes a camera with a frustrum of size (`width` x `height`). */
void camera_init(camera_t *, vec2s pos, int width, int height);

/* Updates the camera's view matrix. */
void camera_update_view(camera_t *);

#endif // ifndef _GAME_CAMERA_H_
