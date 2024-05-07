#include "game/buttons.h"
#include "game/game.h"

#include "graphics/color.h"
#include "graphics/drawing.h"
#include "graphics/renderer.h"

#include <string.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

static void monitor_input(game_s *game) {
    if (button_pressed(SDL_SCANCODE_Q, 0)) {
        game->quit = true;
    }
}

static void poll_events(game_s *game) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                buttons_update(ev.key);
                break;
            case SDL_QUIT:
                game->quit = true;
                break;
            case SDL_WINDOWEVENT:
                if (ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    window_resize(&game->window, ev.window.data1, ev.window.data2);
                break;
            default:
                break;
        }
    }
}

static void init(game_s *game) {
    memset(game, 0, sizeof(game_s));
    window_init(&game->window, "Game");
    renderer_init();
    time_init(&game->time);
}

static void destroy(game_s *game) {
    window_destroy(&game->window);
    renderer_destroy();
}

static void update(game_s *game) {
    time_update(&game->time);
}

static void tick(game_s *game) {
    if (!time_tick(&game->time))
        return;
}

static void render(game_s *game) {
    draw_quad((vec2s) { 0 }, (vec2s) { 0 }, COLOR_WHITE);
}

int main(int argc, char *argv[]) {
    game_s game;
    init(&game);

    while (!game.quit) {
        poll_events(&game);
        monitor_input(&game);

        renderer_prepare_scene();
        {
            update(&game);
            tick(&game);
            render(&game);
        }
        renderer_present_scene();

        window_swap_buffers(&game.window);
    }

    destroy(&game);

    return 0;
}
