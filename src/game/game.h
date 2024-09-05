/**
 * @brief game.h
 * @author Justin Tonkinson
 * @date 2024/05/06
 * @brief Game function prototypes.
 */

#ifndef _GAME_GAME_H_
#define _GAME_GAME_H_

#include "graphics/window.h"
#include "level/editor.h"
#include "util/time.h"
#include "world/world.h"

#include <stdbool.h>

/** @brief All possible game states. */
typedef enum game_state_e {
    GS_QUIT = -1,
    GS_DEAD,
    GS_PAUSED,
    GS_PLAYING,
    GS_EDIT,
} game_state_t;

/** @brief Game data structure. */
typedef struct game_s {
    /** @brief Boolean wireframe variable. */
    bool wireframe;

    /** @brief The previous game state. */
    game_state_t prev_state;

    /** @brief The current game state. */
    game_state_t state;

    /** @brief The game window. */
    window_t window;

    /** @brief Time management structure. */
    struct time_s time;

    /** @brief The game world. */
    world_t world;

    /** @brief The level editor. */
    editor_t editor;
} game_t;

/**
 * @brief Initializes the game and starts the game loop. 
 * @param self a pointer to the game
 */
void game_run(game_t *);

#endif // ifndef _GAME_GAME_H_
