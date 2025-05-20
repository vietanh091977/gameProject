#pragma once

#ifndef GAME_H
#define GAME_H

#include "renderWindow.h"

enum GameState {
    MENU,
    PLAYING,
    PAUSING,
    GAME_OVER
};

class Game {
public:
    Game();
    ~Game();
    GameState gameState;
    ButtonState playState;
    ButtonState levelState;
    ButtonState pauseState;
    ButtonState resumeState;
    ButtonState quitState;
    ButtonState homeState;
    ButtonState replayState;
    bool isRunning();
    bool GameMenu();
    bool GamePlay();
    bool GamePause();
    bool GameOver();
    void run();

private:
    void resetGame();
    void handleEvents();
    void update(float deltaTime);
    void render();
    void nextTetromino();
    void reset();
    void handleHoldBlock();
    void updateGhostBlock();
    void checkGameOver();
    bool checkCollision();

    int grid[GRID_HEIGHT][GRID_WIDTH];
    struct Point { int x, y; } current[4], backup[4], ghost[4];   // current block, backup block and ghost block
    int figures[7][4];
    int dx;
    int color;
    int holdBlock;
    int linesCleared;
    int linesClearedAtOnce;
    int score;
    int level;
    bool running;
    bool gameMenuRunning;
    bool gameRunning;
    bool gamePausing;
    bool gameOverRunning;
    bool hardDrop;
    bool rotate;
    bool hasHeld;
    bool holdUsed;
    bool musicOn;
    bool SFXon;
    float timer;
    float delay;

    int bagIndex;
    void refillBag();
    void initQueue();
    std::vector<int> bag;
    std::vector<int> highScores;
        
    RenderWindow* renderWindow;
    SDL_Texture* tileTexture;
    SDL_Texture* ghostTexture;
};

#endif
