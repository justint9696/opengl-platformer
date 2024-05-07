#pragma once

#include <SDL2/SDL_video.h>
#include <stdbool.h>

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

typedef struct {
    int width, height;
    SDL_Window *handle;
    SDL_GLContext context;
} window_s;

void window_init(window_s *, const char *title);
void window_destroy(window_s *);

void window_resize(window_s *, int width, int height);
void window_swap_buffers(window_s *);
