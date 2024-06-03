#include "entity/entity.h"
#include "game/buttons.h"
#include "util/time.h"
#include "world/world.h"

void input_tick(entity_t *self, world_t *world, float dt) {
    if (button_pressed(SDL_SCANCODE_A, 0)) {
        self->body.direction.x += -1.f;
    } 
    if (button_pressed(SDL_SCANCODE_D, 0)) {
        self->body.direction.x += 1.f;
    } 

    if (button_pressed(SDL_SCANCODE_SPACE, 0) && self->body.grounded) {
        self->body.direction.y += 1.f;
        self->body.grounded = false;
    }

    if (button_pressed(SDL_SCANCODE_RSHIFT, 1000) && !self->body.grounded) {
        self->body.dash_tick = NOW();
    }
}
