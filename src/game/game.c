#include "game/game.h"

#include "game/buttons.h"
#include "graphics/renderer.h"
#include "ui/ui.h"
#include "util/io.h"

#include <glad/glad.h>
#include <string.h>

static void monitor_input(game_t *game) {
    if (button_pressed(SDL_SCANCODE_Q, 0)) {
        game->quit = true;
    }
    if (button_pressed(SDL_SCANCODE_T, 250)) {
        game->wireframe ^= 1;
        if (game->wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

static void poll_events(game_t *game) {
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
                    window_resize(&game->window, ev.window.data1,
                                  ev.window.data2);
                break;
            default:
                break;
        }
    }
}

static void init(game_t *self) {
    memset(self, 0, sizeof(game_t));
    window_init(&self->window, "Game");
    buttons_init();
    time_init(&self->time);
    world_init(&self->world);
    ui_init();
}

static void destroy(game_t *self) {
    window_destroy(&self->window);
    world_destroy(&self->world);
}

static void update(game_t *self) {
    time_update(&self->time);
    world_update(&self->world, self->time.delta);
}

static void tick(game_t *self) {
    while (time_tick(&self->time)) {
        world_tick(&self->world, self->time.delta_fixed);
    }
}

static void render(game_t *self) {
    world_render(&self->world);
    ui_render(&self->world.camera);
}

void game_run(game_t *self) {
    init(self);
    while (!self->quit) {
        if (time_since(self->time.last_second) > NS_PER_SECOND) {
            window_title(&self->window, "FPS: %d TPS: %d", self->time.fps,
                         self->time.tps);
            LOG("%s\n", self->window.title);
        }

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
