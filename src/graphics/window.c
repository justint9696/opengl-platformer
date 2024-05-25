#include "graphics/window.h"
#include "graphics/renderer.h"

#include <SDL2/SDL.h>
#include <glad/glad.h>

void window_init(window_t *self, const char *title) {
    memset(self, 0, sizeof(window_t));

    self->width = SCREEN_WIDTH;
    self->height = SCREEN_HEIGHT;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "ERROR: Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    // create window
    self->handle =
        SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if (!self->handle) {
        fprintf(stderr, "ERROR: Could not create SDL Window: %s\n", SDL_GetError());
        exit(1);
    }

    self->context =  SDL_GL_CreateContext(self->handle);
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        fprintf(stderr, "ERROR: Failed to initialize GLAD\n");
        exit(1);
    }

    // disble vsync
    if (SDL_GL_SetSwapInterval(0) < 0) {
        fprintf(stderr, "WARNING: Could not disable VSync: %s\n", SDL_GetError());
    }

    renderer_init();
}

void window_destroy(window_t *self) {
    // destroy renderer
    renderer_destroy();

    // destroy window
    SDL_GL_DeleteContext(self->context);
    SDL_DestroyWindow(self->handle);
    SDL_Quit();
}

void window_resize(window_t *self, int width, int height) {
    self->width = width; 
    self->height = height;
    glViewport(0, 0, width, height);
}

void window_swap_buffers(window_t *self) {
    SDL_GL_SwapWindow(self->handle);
}
