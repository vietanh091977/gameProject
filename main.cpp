//#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include "renderWindow.h"
#include "game.h"

int main(int argc, char* argv[]) {
    Game game;
    game.run();

    return 0;
}
