#include "entity/entity.h"
#include "game/constants.h"
#include "game/input.h"
#include "util/time.h"
#include "world/world.h"

#include <cglm/struct.h>

void input_sync(entity_t *self, world_t *world, float dt) {
    if (button_held(SDL_SCANCODE_A, 0)) {
        self->body.vdir.x += -1.f;
    }

    if (button_held(SDL_SCANCODE_D, 0)) {
        self->body.vdir.x += 1.f;
    }

    if (button_held(SDL_SCANCODE_SPACE, 0) && self->body.grounded) {
        self->body.vdir.y += 1.f;
        self->body.grounded = false;
    }

    if (button_pressed(SDL_SCANCODE_RSHIFT) && !self->body.grounded
        && time_since_ms(self->body.dash_tick) > DASH_COOLDOWN) {
        self->body.dash_tick = NOW();
    }
}
