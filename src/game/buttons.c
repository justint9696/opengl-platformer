#include "game/buttons.h"
#include "util/time.h"

static keyboard_s keyboard;

void buttons_init() {
    memset(&keyboard, 0, sizeof(keyboard_s));
}

void buttons_update(SDL_KeyboardEvent ev) {
    keyboard.buttons[ev.keysym.scancode] = (button_s) {
        .pressed = ev.state == SDL_PRESSED,
        .tick = NOW(),
    };
}

bool button_pressed(SDL_Scancode scancode, time_t delay_ms) {
    const time_t now = NOW();
    button_s *btn = &keyboard.buttons[scancode];
    if (!btn->pressed || now - btn->tick < NS(delay_ms))
        return false;

    btn->tick = now;
    return true;
}
