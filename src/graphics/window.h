#ifndef _GRAPHICS_WINDOW_H_
#define _GRAPHICS_WINDOW_H_

#include <SDL2/SDL_video.h>
#include <stdbool.h>

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

typedef struct {
    char title[64];
    int width, height;
    SDL_Window *handle;
    SDL_GLContext context;
} window_t;

void window_init(window_t *, const char *title);
void window_destroy(window_t *);

void window_resize(window_t *, int width, int height);
void window_swap_buffers(window_t *);

void window_title(window_t *, const char *format, ...);

#endif
