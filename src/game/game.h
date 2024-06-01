#ifndef _GAME_GAME_H_
#define _GAME_GAME_H_

#include "graphics/window.h"
#include "util/time.h"
#include "world/world.h"

#include <stdbool.h>

typedef struct {
    bool quit;
    bool wireframe;
    window_t window; 
    struct time_s time;
    world_t world;
} game_t;

void game_run(game_t *);

#endif
