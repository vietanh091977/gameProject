#pragma once

#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include "renderWindow.h"

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    void handleEvents();
    void update(float deltaTime);
    void render();
    void resetFigures();
    bool check();

    static const int M = 20;
    static const int N = 10;
    static const int TILE_SIZE = 18;
    static const int TILE_SCALED_SIZE = 27;
    int field[M][N];
    struct Point { int x, y; } a[4], b[4];
    int figures[7][4];
    int dx;
    bool rotate;
    int colorNum;
    float timer;
    float delay;
    bool running;
    RenderWindow* renderWindow;
    SDL_Texture* tileTexture;
    SDL_Texture* backgroundTexture;
    SDL_Texture* frameTexture;
};

#endif
