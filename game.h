#pragma once

#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <unordered_map>
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
    void nextTetromino();
    void reset();
    void handleHoldBlock();
    void updateGhostBlock();
    bool checkCollision();
    bool checkGhostCollision();
    void checkGameOver();

    static const int M = 20;
    static const int N = 10;
    int field[M][N];
    struct Point { int x, y; } a[4], b[4], ghost[4];
    int figures[7][4];
    int dx;
    int holdBlock;
    int linesCleared;
    int linesClearedAtOnce;
    bool running;
    bool hardDrop;
    bool rotate;
    bool hasHeld;
    bool holdUsed;
    int color;
    float timer;
    float delay;

    int bagIndex;
    void refillBag();
    void initQueue();
    std::vector<int> bag;
        
    RenderWindow* renderWindow;
    SDL_Texture* tileTexture;
    SDL_Texture* backgroundTexture;
    SDL_Texture* matrixTexture;
    SDL_Texture* ghostTexture;
};

#endif
