#pragma once

#include "graphics/window.h"
#include "util/time.h"
#include "world/world.h"

#include <stdbool.h>

typedef struct {
    bool quit: 1;
    bool debug: 1;
    window_s window; 
    time_s time;
    world_s world;
} game_s;

void game_run(game_s *);
