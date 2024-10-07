/**
 * @file window.h
 * @author Justin Tonkinson
 * @date 2024/05/06
 * @brief Window data structures and function prototypes.
 */

#ifndef _GRAPHICS_WINDOW_H_
#define _GRAPHICS_WINDOW_H_

#include <SDL_video.h>
#include <stdbool.h>

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

typedef struct {
    char title[64];
    int width, height;
    SDL_Window *handle;
    SDL_GLContext context;
} window_t;

/**
 * @brief Creates a window and initializes its renderer.
 * @param self self a pointer to the window
 * @param title title of the window
 */
void window_init(window_t *, const char *title);

/**
 * @brief Destroys a window and its renderer.
 * @param self a pointer to the window
 */
void window_destroy(window_t *);

/**
 * @brief Resizes a window to the provided @a width and @a height.
 * @param self a pointer to the window
 * @param width the width of the window
 * @param height the height of the window
 */
void window_resize(window_t *, int width, int height);

/**
 * @brief Swaps the window's buffer.
 * @param self a pointer to the window
 */
void window_swap_buffers(window_t *);

/**
 * @brief Changes the window's title.
 * @param self a pointer to the window
 * @param format printf format string
 */
void window_title(window_t *, const char *format, ...);

#endif // ifndef _GRAPHICS_WINDOW_H_
