#include "game/game.h"
#include "util/log.h"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
    const char *fname = NULL;
    if (argc > 1) {
        fname = argv[1];
        log_debug("Loading level: `%s`\n", fname);
    }

    game_t game;
    game_run(&game, fname);
    log_debug("Program exited normally.\n");
    return 0;
}
