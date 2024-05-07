#include "game/camera.h"

#include <cglm/cam.h>
#include <cglm/struct.h>

void camera_init(camera_s *self, int width, int height) {
    float w = width * 0.5f, h = height * 0.5f;
    self->projection = glms_ortho(-w, w, -h, h, -1.f, 1.f);
    self->pos = (vec3s) { 0.f, 0.f, -1.f };
    camera_update_view(self);
}

void camera_update_view(camera_s *self) {
    mat4s transform = glms_translate(glms_mat4_identity(), self->pos);
    transform = glms_rotate_z(transform, 0.f);
    self->view = glms_mat4_inv(transform);
}
