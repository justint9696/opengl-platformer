#include "entity/entity.h"
#include "game/buttons.h"
#include "world/world.h"

void input_tick(entity_s *self, world_s *world, float dt) {
    rigid_body_s *rb = &self->body;
    rb->direction = 0;

    if (button_pressed(SDL_SCANCODE_LEFT, 0) || button_pressed(SDL_SCANCODE_A, 0)) {
        rb->direction |= DIR_LEFT;
    } else if (button_pressed(SDL_SCANCODE_RIGHT, 0) || button_pressed(SDL_SCANCODE_D, 0)) {
        rb->direction |= DIR_RIGHT;
    }
    if (button_pressed(SDL_SCANCODE_UP, 0) || button_pressed(SDL_SCANCODE_W, 0)) {
        rb->direction |= DIR_UP;
    } else if (button_pressed(SDL_SCANCODE_DOWN, 0) || button_pressed(SDL_SCANCODE_S, 0)) {
        rb->direction |= DIR_DOWN;
    } 
}
