#include "game.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

Game::Game() {
    running = true;
    dx = 0;
    rotate = false;
    colorNum = 1;
    timer = 0;
    delay = 0.3f;

    renderWindow = new RenderWindow(WINDOW_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
    tileTexture = renderWindow->loadTexture("images/tiles.png");
    backgroundTexture = renderWindow->loadTexture("images/background.png");
    frameTexture = renderWindow->loadTexture("images/frame.png");

    // Initialize field and figures here
    resetFigures();
}

Game::~Game() {
    SDL_DestroyTexture(tileTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(frameTexture);
    delete renderWindow;
}

void Game::run() {
    srand(static_cast<unsigned int>(time(NULL)));
    Uint32 startTick;

    while (running) {
        startTick = SDL_GetTicks();
        //std::cout << "startTick: " << startTick << std::endl << "Timer: " << timer << ", Delay: " << delay << std::endl;
        handleEvents();
        update((SDL_GetTicks() - startTick) / 1000.0f);
        render();
        SDL_Delay(16);
    }
}

void Game::handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
        else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_UP) rotate = true;
            else if (event.key.keysym.sym == SDLK_LEFT) dx = -1;
            else if (event.key.keysym.sym == SDLK_RIGHT) dx = 1;
        }
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_DOWN]) delay = 0.05f;
}

void Game::update(float deltaTime) {
    //timer += deltaTime;

    // Horizontal movement
    for (int i = 0; i < 4; i++) {
        b[i] = a[i];
        a[i].x += dx;
    }
    if (!check())
        for (int i = 0; i < 4; i++)
            a[i] = b[i];

    // Rotate tetromino
    if (rotate) {
        Point p = a[2]; // Center of rotation
        for (int i = 0; i < 4; i++) {
            int x = a[i].y - p.y;
            int y = a[i].x - p.x;
            a[i].x = p.x - x;
            a[i].y = p.y + y;
        }
        if (!check())
            for (int i = 0; i < 4; i++)
                a[i] = b[i];
    }

    //std::cout << "Timer: " << timer << ", DeltaTime: " << deltaTime << ", Delay: " << delay << std::endl;

    // Move downward
    if (timer > delay) {
        for (int i = 0; i < 4; i++) {
            b[i] = a[i];
            a[i].y += 1;
        }
        if (!check()) {
            for (int i = 0; i < 4; i++)
                field[b[i].y][b[i].x] = colorNum;
            //colorNum = 1 + rand() % 7;
            resetFigures();
        }
        timer = 0;
    }

    // Check and clear completed rows
    int k = M - 1;
    for (int i = M - 1; i >= 0; i--) {
        int count = 0;
        for (int j = 0; j < N; j++) {
            if (field[i][j]) count++;
            field[k][j] = field[i][j];
        }
        if (count < N) k--;
    }

    // Reset movement and rotation flags
    dx = 0;
    rotate = false;
    delay = 0.3f;
    timer += 0.016f;
    //timer += deltaTime;
}


void Game::render() {
    renderWindow->clear();
    renderWindow->render(backgroundTexture, NULL, NULL);

    SDL_Rect src, dest;

    // Draw the playing field
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j] == 0) continue;
            src = { field[i][j] * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE };
            dest = { j * TILE_SIZE + 28, i * TILE_SIZE + 31, TILE_SIZE, TILE_SIZE };
            renderWindow->render(tileTexture, &src, &dest);
        }
    }

    // Draw the active tetromino
    for (int i = 0; i < 4; i++) {
        src = { colorNum * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE };
        dest = { a[i].x * TILE_SIZE + 28, a[i].y * TILE_SIZE + 31, TILE_SIZE, TILE_SIZE };
        renderWindow->render(tileTexture, &src, &dest);
    }

    // Draw the frame
    renderWindow->render(frameTexture, NULL, NULL);

    renderWindow->display();
}


void Game::resetFigures() {

    /*
    0   1   2   3
    4   5   6   7
    */

    int figures[7][4] = {
        {1, 4, 5, 6},   // T - Purple
        {0, 1, 5, 6},   // Z - Red
        {2, 1, 5, 4},   // S - Green
        {1, 2, 5, 6},   // O - Yellow
        {4, 5, 6, 7},   // I - Cyan
        {2, 6, 5, 4},   // L - Orange
        {0, 4, 5, 6},   // J - Blue
    };

    int randNum = rand() % 7;
    colorNum = 1 + randNum % 7;
    int n = randNum % 7; // Randomly select a tetromino type
    for (int i = 0; i < 4; i++) {
        a[i].x = figures[n][i] % 4;
        a[i].y = figures[n][i] / 4;
    }
}


bool Game::check() {
    for (int i = 0; i < 4; i++) {
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return false;
        else if (field[a[i].y][a[i].x]) return false;
    }
    return true;
}
