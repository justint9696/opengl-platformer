#ifndef _GAME_INPUT_H_
#define _GAME_INPUT_H_

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_scancode.h>

#include <cglm/types-struct.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

enum {
    BUTTON_UP     = 1 << 0,
    BUTTON_DOWN   = 1 << 1,
    BUTTON_LEFT   = 1 << 2,
    BUTTON_RIGHT  = 1 << 3,
    BUTTON_SPACE  = 1 << 4,
    BUTTON_LSHIFT = 1 << 5,
    BUTTON_RSHIFT = 1 << 6,
};

typedef enum {
    BS_RELEASED,
    BS_PRESSED,
    BS_HELD,
} button_e;

typedef struct {
    button_e state;
    time_t pressed_tick;
    time_t released_tick;
} button_t;

typedef struct {
    struct {
        button_t buttons[SDL_NUM_SCANCODES];
    } keyboard;
    struct {
        button_t buttons[SDL_BUTTON_X2 + 1];
    } mouse;
} input_t;

void buttons_init();
void buttons_update();

bool button_pressed(SDL_Scancode);
bool button_held(SDL_Scancode, time_t delay_ms);
bool button_released(SDL_Scancode);

bool mouse_pressed(uint32_t);
bool mouse_held(uint32_t, time_t delay_ms);
bool mouse_released(uint32_t);

ivec2s mouse_get_position();

#endif // _GAME_INPUT_H_
