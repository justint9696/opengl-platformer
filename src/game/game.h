#ifndef _GAME_GAME_H_
#define _GAME_GAME_H_

#include "graphics/window.h"
#include "util/time.h"
#include "world/world.h"

#include <stdbool.h>

typedef struct {
    bool quit: 1;
    bool wireframe: 1;
    window_t window; 
    time_s time;
    world_t world;
} game_t;

void game_run(game_t *);

#endif
