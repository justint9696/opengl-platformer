#include <glad/glad.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <stdbool.h>
#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main(int argc, char *argv[]) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Window *window =
        SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if (!window) {
        fprintf(stderr, "Could not create SDL Window: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD: %s\n", SDL_GetError());
        exit(1);
    }

    glDisable(GL_DEPTH_TEST);
    glClearColor(0.53f, 0.81f, 0.92f, 1.f);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    bool quit = false;
    while (!quit) {
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
                case SDL_KEYDOWN:
                    if (ev.key.keysym.scancode == SDL_SCANCODE_Q)
                        quit = true;
                    break;

                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
