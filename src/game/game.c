#include "game/game.h"
#include "game/buttons.h"
#include "game/camera.h"

#include "graphics/color.h"
#include "graphics/drawing.h"
#include "graphics/renderer.h"
#include "graphics/window.h"

#include "util/time.h"

#include <glad/glad.h>
#include <string.h>

static void monitor_input(game_s *game) {
    if (button_pressed(SDL_SCANCODE_Q, 0)) {
        game->quit = true;
    }
    if (button_pressed(SDL_SCANCODE_T, 250)) {
        game->debug ^= 1;
        if (game->debug)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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


static void init(game_s *self) {
    memset(self, 0, sizeof(game_s));
    window_init(&self->window, "Game");
    buttons_init();
    time_init(&self->time);
    camera_init(&self->world.camera, self->window.width, self->window.height);
}

static void destroy(game_s *self) {
    window_destroy(&self->window);
}

static void update(game_s *self) {
    time_update(&self->time);
}

static void tick(game_s *self) {
    if (!time_tick(&self->time))
        return;
}

static void render(game_s *self) {
    draw_quad((vec2s) { 0.f, 0.f }, (vec2s) { 50.f, 50.f }, COLOR_WHITE);
}

void game_run(game_s *self) {
    init(self);
    while (!self->quit) {
        poll_events(self);
        monitor_input(self);
        renderer_prepare_scene(&self->world.camera);
        {
            update(self);
            tick(self);
            render(self);
        }
        renderer_present_scene();
        window_swap_buffers(&self->window);
    }
    destroy(self);
}
