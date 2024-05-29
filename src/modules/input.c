#include "entity/entity.h"
#include "game/buttons.h"
#include "world/world.h"

void input_tick(entity_t *self, world_t *world, float dt) {
    if (button_pressed(SDL_SCANCODE_A, 0)) {
        self->body.movement.x = -1.f;
        /* self->body.vel.x = -self->body.movement_speed; */
    } else if (button_pressed(SDL_SCANCODE_D, 0)) {
        self->body.movement.x = 1.f;
        /* self->body.vel.x = self->body.movement_speed; */
    } else {
        self->body.movement.x = 0.f;
        /* self->body.vel.x = 0.f; */
    }

    if (button_pressed(SDL_SCANCODE_SPACE, 0) && self->body.grounded) {
        self->body.movement.y = 1.f;
        /* self->body.vel.y = self->body.jump_speed; */
    }
}
