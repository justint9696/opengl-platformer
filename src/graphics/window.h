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

/* Creates a window and initializes its renderer. */
void window_init(window_t *, const char *title);

/* Destroys a window and its renderer. */
void window_destroy(window_t *);

/* Resizes a window to the provided `width` and `height`. */
void window_resize(window_t *, int width, int height);

/* Swaps the window's buffer. */
void window_swap_buffers(window_t *);

/* Changes the window's title. */
void window_title(window_t *, const char *format, ...);

#endif // ifndef _GRAPHICS_WINDOW_H_
