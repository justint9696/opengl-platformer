#include "game/input.h"
#include "entity/entity.h"
#include "game/defines.h"
#include "util/time.h"
#include "world/world.h"

#include <cglm/struct.h>

void input_sync(entity_t *self, world_t *world, float dt) {
    self->buttons = 0;

    if (button_held(SDL_SCANCODE_A, 0)) {
        self->buttons |= BUTTON_LEFT;
    }

    if (button_held(SDL_SCANCODE_D, 0)) {
        self->buttons |= BUTTON_RIGHT;
    }

    if (button_held(SDL_SCANCODE_SPACE, 0) && self->body.grounded) {
        self->buttons |= BUTTON_SPACE;
    }

    if (button_pressed(SDL_SCANCODE_RSHIFT) && !self->body.grounded
        && time_since_ms(self->body.dash_tick) > DASH_COOLDOWN) {
        self->buttons |= BUTTON_RSHIFT;
    }
}
