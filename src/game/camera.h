#pragma once

#include <cglm/types-struct.h>

typedef struct {
    vec3s pos;
    mat4s projection, view;
} camera_s;

void camera_init(camera_s *, int width, int height);
void camera_update_view(camera_s *);
