#pragma once

#include "graphics/window.h"
#include "graphics/renderer.h"
#include "util/time.h"

#include <stdbool.h>

typedef struct {
    bool quit: 1;
    window_s window; 
    renderer_s *renderer;
    time_s time;
} game_s;
