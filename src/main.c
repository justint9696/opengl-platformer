#include "game/game.h"
#include "util/log.h"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
    game_t game;
    game_run(&game);
    log_debug("Program exited normally.\n");
    return 0;
}
