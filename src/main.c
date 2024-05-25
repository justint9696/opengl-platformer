#include "game/game.h"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
    game_t game;
    game_run(&game);
    return 0;
}
