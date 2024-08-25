#include "game/input.h"

#include "util/time.h"

#include <stdio.h>

static input_t input;

void buttons_init() {
    memset(&input, 0, sizeof(input));
}

static void update_button_state(button_t *button, bool pressed) {
    const time_t now = NOW();
    if (pressed) {
        switch (button->state) {
            case BS_RELEASED:
                button->state = BS_PRESSED;
                button->pressed_tick = now;
                break;
            case BS_PRESSED:
                button->state = BS_HELD;
                break;
            default:
                break;
        }
    } else {
        button->state = BS_RELEASED;
        button->released_tick = now;
    }
}

void buttons_update() {
    // update all keyboard buttons
    const uint8_t *keyboard = SDL_GetKeyboardState(NULL);
    for (size_t i = 0; i < SDL_NUM_SCANCODES; i++) {
        bool pressed = keyboard[i];
        button_t *button = &input.keyboard.buttons[i];
        update_button_state(button, pressed);
    }

    // update all mouse buttons
    const uint32_t mouse = SDL_GetRelativeMouseState(NULL, NULL);
    for (size_t i = 0; i < SDL_BUTTON_X2; i++) {
        bool pressed = (mouse & SDL_BUTTON(i));
        button_t *button = &input.mouse.buttons[i];
        update_button_state(button, pressed);
    }
}

bool button_pressed(SDL_Scancode scancode) {
    return input.keyboard.buttons[scancode].state == BS_PRESSED;
}

bool button_held(SDL_Scancode scancode, time_t delay_ms) {
    button_t *button = &input.keyboard.buttons[scancode];
    if ((button->state == BS_PRESSED)
        || (button->state && time_since_ms(button->pressed_tick) > delay_ms)) {
        button->pressed_tick = NOW();
        return true;
    }

    return false;
}

bool button_released(SDL_Scancode scancode) {
    const button_t *button = &input.keyboard.buttons[scancode];
    return (button->state == BS_RELEASED
            && time_since(button->released_tick) < NS_PER_MS);
}

bool mouse_pressed(uint32_t index) {
    return (input.mouse.buttons[index].state == BS_PRESSED);
}

bool mouse_held(uint32_t index, time_t delay_ms) {
    button_t *button = &input.mouse.buttons[index];
    if ((button->state == BS_PRESSED)
        || (button->state && time_since_ms(button->pressed_tick) > delay_ms)) {
        button->pressed_tick = NOW();
        return true;
    }

    return false;
}

bool mouse_released(uint32_t index) {
    const button_t *button = &input.mouse.buttons[index];
    return (button->state == BS_RELEASED
            && time_since(button->released_tick) < NS_PER_MS);
}

ivec2s mouse_position() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return (ivec2s) { .x = x, .y = y };
}
