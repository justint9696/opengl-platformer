/**
 * @file input.h
 * @author Justin Tonkinson
 * @date 2024/05/07
 * @brief Input prototype functions.
 */

#ifndef _GAME_INPUT_H_
#define _GAME_INPUT_H_

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_scancode.h>

#include <cglm/types-struct.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

/** @brief Entity button bits. */
enum {
    BUTTON_UP     = 1 << 0,
    BUTTON_DOWN   = 1 << 1,
    BUTTON_LEFT   = 1 << 2,
    BUTTON_RIGHT  = 1 << 3,
    BUTTON_SPACE  = 1 << 4,
    BUTTON_LSHIFT = 1 << 5,
    BUTTON_RSHIFT = 1 << 6,
};

/** @brief All possible button states. */
typedef enum {
    BS_RELEASED,
    BS_PRESSED,
    BS_HELD,
} button_state_t;

/** @brief Individual button data structure. */
typedef struct {
    button_state_t state;
    time_t pressed_tick;
    time_t released_tick;
} button_t;

/** @brief Button input manager. */
typedef struct input_s {
    struct {
        button_t buttons[SDL_NUM_SCANCODES];
    } keyboard;
    struct {
        button_t buttons[SDL_BUTTON_X2 + 1];
    } mouse;
} input_t;

/**
 * @brief Initializes the internal button state. 
 */
void buttons_init();

/** 
 * @brief Updates the internal button state. 
 */
void buttons_update();

/** 
 * @brief Returns true if the button state is pressed. 
 * @param scancode SDL keyboard scancode
 * @returns true if button is pressed
 */
bool button_pressed(SDL_Scancode);

/** 
 * @brief Returns true if the button state is held after a certain time. 
 * @param scancode SDL keyboard scancode
 * @param delay_ms time to wait since last button update
 * @returns true if button is held after the given delay
 */
bool button_held(SDL_Scancode, time_t delay_ms);

/**
 * @brief Returns true if the button state is released. 
 * @param scancode SDL keyboard scancode
 * @returns true if the button is released
 */
bool button_released(SDL_Scancode);

/**
 * @brief Returns true if the button state is pressed. 
 * @param index SDL mouse button index
 * @returns true if the button is pressed
 */
bool mouse_pressed(uint32_t);

/**
 * @brief Returns true if the button state is held after a certain time. 
 * @param index SDL mouse button index
 * @param delay_ms time to wait since last button update
 * @returns true if button is held after the given delay
 */
bool mouse_held(uint32_t, time_t delay_ms);

/**
 * @brief Returns true if the button state is released. 
 * @param index SDL mouse button index
 * @returns true if the button is released
 */
bool mouse_released(uint32_t);

/**
 * @brief Returns the screen position of the mouse. 
 * @returns the screen position of the mouse
 */
ivec2s mouse_position();

#endif // ifndef _GAME_INPUT_H_
