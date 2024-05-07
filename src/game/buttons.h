#pragma once

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_scancode.h>

#include <stdbool.h>
#include <time.h>

typedef struct {
    bool pressed;
    time_t tick;
} button_s;

typedef struct {
    button_s buttons[SDL_NUM_SCANCODES];
} keyboard_s;

void buttons_init();
void buttons_update(SDL_KeyboardEvent);

bool button_pressed(SDL_Scancode, time_t delay_ms);
