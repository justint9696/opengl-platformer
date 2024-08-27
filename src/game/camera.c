/**
 * @brief camera.c
 * @author Justin Tonkinson
 * @date 2024/05/07
 * @brief Camera implementation functions.
 */

#include "game/camera.h"

#include <cglm/cam.h>
#include <cglm/struct.h>

void camera_init(camera_t *self, vec2s pos, int width, int height) {
    float w = width * 0.5f, h = height * 0.5f;
    self->projection = glms_ortho(-w, w, -h, h, -1.f, 1.f);
    self->pos = glms_vec3_negate((vec3s) { pos.x, pos.y, 0.f });
    camera_update_view(self);
}

void camera_update_view(camera_t *self) {
    mat4s transform
        = glms_translate(GLMS_MAT4_IDENTITY, (vec3s) { 0.f, 0.f, -1.f });
    transform = glms_rotate_z(transform, 0.f);
    self->view = glms_mat4_inv(transform);
}
