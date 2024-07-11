#include "game/game.h"

#include "game/input.h"
#include "graphics/renderer.h"
#include "ui/ui.h"

#include <cglm/struct.h>
#include <glad/glad.h>
#include <string.h>

static void monitor_input(game_t *game) {
    if (button_pressed(SDL_SCANCODE_Q)) {
        game->state = GS_QUIT;
    }

    if (button_pressed(SDL_SCANCODE_T)) {
        game->wireframe ^= 1;
        if (game->wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (button_pressed(SDL_SCANCODE_RSHIFT) && button_pressed(SDL_SCANCODE_E)) {
        switch (game->state) {
            case GS_EDIT:
                game->state = game->prev_state;
                break;
            default:
                game->prev_state = game->state;
                game->state = GS_EDIT;
                break;
        }
    }
}

static void poll_events(game_t *game) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
            case SDL_QUIT:
                game->state = GS_QUIT;
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
    level_editor_init(&self->editor);
}

static void destroy(game_t *self) {
    window_destroy(&self->window);
    world_destroy(&self->world);
}

static void update(game_t *self) {
    time_update(&self->time);
    world_update(&self->world, self->time.delta);
    level_editor_update(&self->editor, &self->world);
}

static void sync(game_t *self) {
    while (time_sync(&self->time)) {
        buttons_update();
        monitor_input(self);

        // synchonize game
        world_sync(&self->world, self->time.delta_fixed);
    }
}

static void render(game_t *self) {
    world_render(&self->world);
    ui_render(&self->world.camera);
}

void game_run(game_t *self) {
    // initialize game
    init(self);

    level_import(&self->level, &self->world, "data/demo.dat");
    /* level_export(&self->level, &self->world, "data/demo.dat"); */

    while (self->state != GS_QUIT) {
        poll_events(self);

        renderer_prepare_scene(&self->world.camera);
        {
            update(self);
            sync(self);
            render(self);
        }
        renderer_present_scene();

        window_swap_buffers(&self->window);
    }

    // deinit game
    destroy(self);
}
