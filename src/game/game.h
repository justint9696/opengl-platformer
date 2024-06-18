#ifndef _GAME_GAME_H_
#define _GAME_GAME_H_

#include "graphics/window.h"
#include "level/editor.h"
#include "level/level.h"
#include "util/time.h"
#include "world/world.h"

#include <stdbool.h>

typedef enum {
    GS_QUIT = -1,
    GS_DEAD,
    GS_PAUSED,
    GS_PLAYING,
    GS_EDIT,
} game_state_t;

typedef struct {
    bool wireframe;
    game_state_t prev_state, state;
    window_t window;
    struct time_s time;
    world_t world;
    level_t level;
    level_editor_t editor;
} game_t;

void game_run(game_t *);

#endif
