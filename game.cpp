#include "game.h"
#include <cstdlib>
#include <ctime>
#include <random>
#include <deque>
#include <iostream>

Game::Game() {
    srand(static_cast<unsigned int>(time(NULL)));
    running = true;
    dx = 0;
    linesCleared = 0;
    rotate = false;
    hardDrop = false;
    hasHeld = false;
    holdUsed = false;
    color = 1;
    timer = 0;
    delay = 0.3f;
    bagIndex = 0;

    renderWindow = new RenderWindow(WINDOW_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
    tileTexture = renderWindow->loadTexture("images/tiles.png");
    backgroundTexture = renderWindow->loadTexture("images/background.png");
    matrixTexture = renderWindow->loadTexture("images/matrix.png");
    ghostTexture = renderWindow->loadTexture("images/ghostTiles.png");
    renderWindow->loadAssets();

    refillBag();
    initQueue();
    nextTetromino();
}

Game::~Game() {
    SDL_DestroyTexture(tileTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(matrixTexture);
    delete renderWindow;
}

void Game::run() {
    //srand(static_cast<unsigned int>(time(NULL)));
    Uint32 startTick;

    while (running) {
        startTick = SDL_GetTicks();
        if (!running) {
            std::cout << "Game Over!" << std::endl;
            return; 
        }
        //std::cout << "startTick: " << startTick << std::endl << "Timer: " << timer << ", Delay: " << delay << std::endl;
        checkGameOver();
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
            else if (event.key.keysym.sym == SDLK_SPACE) hardDrop = true;
            else if (event.key.keysym.sym == SDLK_c) {
                if (!holdUsed) {
                    handleHoldBlock();
                    holdUsed = true;
                }
            }
        }
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_DOWN]) delay = 0.02f;
}

void Game::update(float deltaTime) {
    if (!running) return;
    //timer += deltaTime;

    updateGhostBlock();

    // Horizontal movement
    for (int i = 0; i < 4; i++) {
        b[i] = a[i];
        a[i].x += dx;
    }
    if (!checkCollision())
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
        if (!checkCollision())
            for (int i = 0; i < 4; i++)
                a[i] = b[i];
    }

    if (hardDrop) {
        while (checkCollision()) {
            for (int i = 0; i < 4; i++) a[i].y++;
        }

        for (int i = 0; i < 4; i++) a[i].y--;
        for (int i = 0; i < 4; i++) {
            field[a[i].y][a[i].x] = color;
        }
        nextTetromino();
        hardDrop = false;
        holdUsed = false;
    }

    //std::cout << "Timer: " << timer << ", DeltaTime: " << deltaTime << ", Delay: " << delay << std::endl;

    // Move downward
    else if (timer > delay) {
        for (int i = 0; i < 4; i++) {
            b[i] = a[i];
            a[i].y += 1;
        }
        if (!checkCollision()) {
            for (int i = 0; i < 4; i++) {
                field[b[i].y][b[i].x] = color;
            }
            nextTetromino();
            holdUsed = false;
        }
        timer = 0;
    }

    // Check and clear completed rows
    int k = M - 1;
    linesClearedAtOnce = 0;
    for (int i = M - 1; i >= 0; i--) {
        int count = 0;
        for (int j = 0; j < N; j++) {
            if (field[i][j]) count++;
            field[k][j] = field[i][j];
        }
        if (count == N) linesClearedAtOnce++; 
        if (count < N) k--;
    }
    linesCleared += linesClearedAtOnce;

    // Reset movement and rotation flags
    dx = 0;
    rotate = false;
    delay = 0.3f;
    timer += 0.016f;
    //timer += deltaTime;

    updateGhostBlock();
    if (linesClearedAtOnce > 0) {
        std::cout << "Lines cleared at once: " << linesClearedAtOnce << std::endl;
    }
    std::cout << linesCleared << std::endl;
}

std::deque<int> queue;

void Game::render() {
    renderWindow->clear();
    renderWindow->render(backgroundTexture, NULL, NULL);
    //updateGhostBlock();

    // Draw the frame
    SDL_Rect matrixDest = { 264, 33, 272, 534 };
    renderWindow->render(matrixTexture, NULL, &matrixDest);
    if (hasHeld) {
        renderWindow->renderHoldBlock(holdBlock);
    }

    for (int i = 0; i < 3; i++) {
        renderWindow->renderNextBlock(queue[i] + 1, i);
    }

    SDL_Rect src, dest;

    // Draw the playing field
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j] == 0) continue;
            src = { field[i][j] * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE };
            dest = { j * TILE_SIZE + 269, i * TILE_SIZE + 39, TILE_SIZE, TILE_SIZE };
            renderWindow->render(tileTexture, &src, &dest);
        }
    }

    // Draw the ghost block
    for (int i = 0; i < 4; i++) {
        src = { color * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE };
        dest = { ghost[i].x * TILE_SIZE + 269, ghost[i].y * TILE_SIZE + 39, TILE_SIZE, TILE_SIZE };
        renderWindow->render(ghostTexture, &src, &dest);
    }

    // Draw the active tetromino
    for (int i = 0; i < 4; i++) {
        src = { color * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE };
        dest = { a[i].x * TILE_SIZE + 269, a[i].y * TILE_SIZE + 39, TILE_SIZE, TILE_SIZE };
        renderWindow->render(tileTexture, &src, &dest);
    }

    renderWindow->display();
}

void Game::refillBag() {
    bag = { 0, 1, 2, 3, 4, 5, 6 };
    unsigned seed = std::time(0);
    std::shuffle(bag.begin(), bag.end(), std::default_random_engine(seed));
    bagIndex = 0;
}

void Game::initQueue() {
    for (int i = 0; i < 3; i++) {
        if (bagIndex >= bag.size()) refillBag();
        queue.push_back(bag[bagIndex++]);
    }
}

void Game::nextTetromino() {

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

    int num = queue.front();
    queue.pop_front();
    color = num + 1;

    for (int i = 0; i < 4; i++) {
        a[i].x = figures[num][i] % 4 + N / 2 - 1;
        a[i].y = figures[num][i] / 4;
    }

    if (bagIndex >= bag.size()) refillBag();
    queue.push_back(bag[bagIndex++]);
}


bool Game::checkCollision() {
    for (int i = 0; i < 4; i++) {
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return false;
        else if (field[a[i].y][a[i].x]) return false;
    }
    return true;
}

bool Game::checkGhostCollision() {
    for (int i = 0; i < 4; i++) {
        if (ghost[i].x < 0 || ghost[i].x >= N || ghost[i].y >= M) return false;
        else if (field[ghost[i].y][ghost[i].x]) return false;
    }
    return true;
}

void Game::updateGhostBlock() {
    // Sao chép vị trí hiện tại của khối
    for (int i = 0; i < 4; i++) {
        ghost[i] = a[i];
    }

    // Di chuyển ghost xuống tối đa
    while (true) {
        for (int i = 0; i < 4; i++) {
            ghost[i].y++;
        }
        if (!checkGhostCollision()) {
            // Nếu ghost không thể di chuyển nữa, lùi lại một bước
            for (int i = 0; i < 4; i++) {
                ghost[i].y--;
            }
            break;
        }
    }
}

void Game::reset() {

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

    for (int i = 0; i < 4; i++) {
        a[i].x = figures[color - 1][i] % 4 + N / 2 - 1;
        a[i].y = figures[color - 1][i] / 4;
    }
}

void Game::handleHoldBlock() {
    if (!hasHeld) {
        holdBlock = color;
        nextTetromino();
        holdUsed = false;
        hasHeld = true;
    }
    else {
        std::swap(color, holdBlock);
        reset();
    }
}

void Game::checkGameOver() {
    int count = 0;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j]) {
                count++;
                break;
            }
        }
    }
    if (count == M) {
        running = false;
    }
}