#ifndef _GAME_CAMERA_H_
#define _GAME_CAMERA_H_

#include <cglm/types-struct.h>

typedef struct {
    vec3s pos;
    mat4s projection, view;
} camera_t;

void camera_init(camera_t *, int width, int height);
void camera_update_view(camera_t *);

#endif
