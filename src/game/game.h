#ifndef _GAME_GAME_H_
#define _GAME_GAME_H_

#include "graphics/window.h"
#include "level/editor.h"
#include "util/time.h"
#include "world/world.h"

#include <stdbool.h>

typedef enum {
    GS_QUIT = -1,
    GS_DEAD,
    GS_PAUSED,
    GS_PLAYING,
    GS_EDIT,
} game_state_e;

typedef struct {
    bool wireframe;
    game_state_e prev_state, state;
    window_t window;
    struct time_s time;
    world_t world;
    editor_t editor;
} game_t;

void game_run(game_t *);

#endif // _GAME_GAME_H_
