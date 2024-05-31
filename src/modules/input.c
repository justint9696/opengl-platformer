#include "entity/entity.h"
#include "game/buttons.h"
#include "world/world.h"

void input_tick(entity_t *self, world_t *world, float dt) {
    if (button_pressed(SDL_SCANCODE_A, 0)) {
        self->body.movement.x = -1.f;
    } else if (button_pressed(SDL_SCANCODE_D, 0)) {
        self->body.movement.x = 1.f;
    } else {
        self->body.movement.x = 0.f;
    }

    if (button_pressed(SDL_SCANCODE_SPACE, 0) && self->body.grounded) {
        self->body.movement.y = 1.f;
    }
}
