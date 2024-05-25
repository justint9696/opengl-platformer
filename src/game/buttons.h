#ifndef _GAME_BUTTONS_H_
#define _GAME_BUTTONS_H_

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_scancode.h>

#include <stdbool.h>
#include <time.h>

typedef struct {
    bool pressed;
    time_t tick;
} button_t;

typedef struct {
    button_t buttons[SDL_NUM_SCANCODES];
} keyboard_t;

void buttons_init();
void buttons_update(SDL_KeyboardEvent);

bool button_pressed(SDL_Scancode, time_t delay_ms);

#endif
