#include "game.h"

Game::Game() {
    srand(static_cast<unsigned int>(time(NULL)));
    running = true;
    gameRunning = true;
    gameMenuRunning = true;
    gamePausing = true;
    gameOverRunning = true;
    dx = 0;
    linesCleared = 0;
    score = 0;
    level = 1;
    rotate = false;
    hardDrop = false;
    hasHeld = false;
    holdUsed = false;
    color = 1;
    timer = 0;
    delay = 0.3f;
    bagIndex = 0;
    gameState = MENU;
    musicOn = true;
    SFXon = true;

    renderWindow = new RenderWindow(WINDOW_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
    tileTexture = renderWindow->loadTexture("assets/images/tiles.png");
    ghostTexture = renderWindow->loadTexture("assets/images/ghostTiles.png");
    renderWindow->loadHighScores("highscore.txt", highScores);
    renderWindow->loadAssets();
    renderWindow->SetRect();

    refillBag();
    initQueue();
    nextTetromino();
}

bool Game::isRunning() {
    return running;
}

void Game::resetGame() {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            field[i][j] = 0;
        }
    }
    nextTetromino();
    hasHeld = false;
    holdUsed = false;
    dx = 0;
    score = 0;
    linesCleared = 0;
    //level = 1;
    timer = 0;
}

void Game::run() {
    while (isRunning()) {
        std::cout << gameState << std::endl;
        switch (gameState) {
            case MENU:
                GameMenu();
                break;
            case PLAYING:
                GamePlay();
                break;
            case PAUSING:
                GamePause();
                break;
            case GAME_OVER:
                GameOver();
                break;
        }
    }
}


bool Game::GameMenu() {
    gameRunning = true;
    gamePausing = true;
    gameOverRunning = true;
    level = 1;
    resetGame();
    Mix_HaltMusic();

    SDL_Event event;
    const SDL_Rect& playRect = renderWindow->getButtonPlayRect();
    const SDL_Rect& levelRect = renderWindow->getButtonLevelRect();

    playState = NORMAL;
    levelState = NORMAL;

    while (gameMenuRunning) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_Point mousePoint = { mx, my };

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                gameMenuRunning = false;
                break;

            case SDL_MOUSEMOTION:
                playState = SDL_PointInRect(&mousePoint, &playRect) ? HOVER : NORMAL;
                levelState = SDL_PointInRect(&mousePoint, &levelRect) ? HOVER : NORMAL;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (SDL_PointInRect(&mousePoint, &playRect)) playState = DOWN;
                    if (SDL_PointInRect(&mousePoint, &levelRect)) levelState = DOWN;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (playState == DOWN && SDL_PointInRect(&mousePoint, &playRect)) {
                        gameState = PLAYING;
                        renderWindow->playMusic(musicOn);
                        gameMenuRunning = false;
                    }

                    if (levelState == DOWN && SDL_PointInRect(&mousePoint, &levelRect)) {
                        level++;
                        if (level == 4) level = 1;
                    }

                    // Reset trạng thái
                    playState = NORMAL;
                    levelState = NORMAL;
                }
                break;
            }
        }
        renderWindow->renderMenuScreen(playState, levelState, level, highScores);
    }

    return gameMenuRunning;
}

bool Game::GamePlay() {
    gameMenuRunning = true;
    gamePausing = true;
    gameOverRunning = true;
    gameRunning = true;

    Uint32 startTick, frameTime;
    const float targetFrameTime = 16.0f; // ~ 60 FPS

    while (gameRunning) {
        startTick = SDL_GetTicks();

        checkGameOver();
        handleEvents();
        update(targetFrameTime / 1000.0f);
        render();

        frameTime = SDL_GetTicks() - startTick;
        if (frameTime < targetFrameTime) {
            SDL_Delay(targetFrameTime - frameTime);
        }
    }

    return gameRunning;
}

void Game::handleEvents() {
    SDL_Event event;

    int mx, my;
    SDL_GetMouseState(&mx, &my);
    SDL_Point mousePoint = { mx, my };

    const SDL_Rect& pauseRect = renderWindow->getButtonPauseRect();

    while (SDL_PollEvent(&event)) {

        switch (event.type) {
            case SDL_QUIT:
                running = false;
                gameRunning = false;
                break;
            case SDL_MOUSEMOTION:
                pauseState = SDL_PointInRect(&mousePoint, &pauseRect) ? HOVER : NORMAL;
                //std::cout << "pauseState: " << pauseState << std::endl;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (SDL_PointInRect(&mousePoint, &pauseRect)) pauseState = DOWN;
                    //std::cout << "pauseState: " << pauseState << std::endl;
                    //std::cout << "Pause button rect: x=" << pauseRect.x << ", y=" << pauseRect.y
                    //    << ", w=" << pauseRect.w << ", h=" << pauseRect.h << std::endl;
                    //std::cout << "Mouse position: x=" << mx << ", y=" << my << std::endl;
                    //std::cout << SDL_PointInRect(&mousePoint, &pauseRect) << std::endl;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    std::cout << "pauseState: " << pauseState << std::endl;
                    std::cout << SDL_PointInRect(&mousePoint, &pauseRect) << std::endl;
                    if (pauseState == DOWN && SDL_PointInRect(&mousePoint, &pauseRect)) {
                        std::cout << "pauseState: " << pauseState << std::endl;
                        gameState = PAUSING;
                        Mix_PauseMusic();
                        gameRunning = false;
                    }
                    pauseState = NORMAL;
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_UP) rotate = true;
                else if (event.key.keysym.sym == SDLK_LEFT) {
                    dx = -1;
                    renderWindow->playSoundEffect(MOVE, SFXon);
                }
                else if (event.key.keysym.sym == SDLK_RIGHT) {
                    dx = 1;
                    renderWindow->playSoundEffect(MOVE, SFXon);
                }
                else if (event.key.keysym.sym == SDLK_DOWN) {
                    delay = 0.02f;
                    renderWindow->playSoundEffect(MOVE, SFXon);
                }
                else if (event.key.keysym.sym == SDLK_SPACE) hardDrop = true;
                else if (event.key.keysym.sym == SDLK_c) {
                    if (!holdUsed) {
                        renderWindow->playSoundEffect(HOLD, SFXon);
                        handleHoldBlock();
                        holdUsed = true;
                    }
                }
                else if (event.key.keysym.sym == SDLK_z) {   // Turn on/off music
                    musicOn = !musicOn;
                    if (Mix_PausedMusic() == 1) {
                        Mix_ResumeMusic();
                    }
                    else {
                        Mix_PauseMusic();
                    }
                }
                else if (event.key.keysym.sym == SDLK_x) {   // Turn on/off sound effect
                    SFXon = !SFXon;
                }
        }
    }
}

void Game::update(float deltaTime) {

    timer += deltaTime;

    updateGhostBlock();

    // Di chuyển ngang
    for (int i = 0; i < 4; i++) {
        b[i] = a[i];
        a[i].x += dx;
    }
    if (!checkCollision())
        for (int i = 0; i < 4; i++)
            a[i] = b[i];

    // Xoay các khối
    if (rotate) {
        Point p = a[2]; // Tâm xoay
        for (int i = 0; i < 4; i++) {
            int x = a[i].y - p.y;
            int y = a[i].x - p.x;
            a[i].x = p.x - x;
            a[i].y = p.y + y;
        }
        if (!checkCollision()) {
            for (int i = 0; i < 4; i++) {
                a[i] = b[i];
            }
        }
        else {
            renderWindow->playSoundEffect(ROTATE, SFXon);
        }
    }

    if (hardDrop) {
        while (checkCollision()) {
            for (int i = 0; i < 4; i++) a[i].y++;
        }

        for (int i = 0; i < 4; i++) a[i].y--;
        for (int i = 0; i < 4; i++) {
            field[a[i].y][a[i].x] = color;
        }
        score += 10 * (level + 1);
        renderWindow->playSoundEffect(HARD_DROP, SFXon);
        nextTetromino();
        hardDrop = false;
        holdUsed = false;
    }

    // Di chuyển dọc xuống
    else if (timer > delay) {
        for (int i = 0; i < 4; i++) {
            b[i] = a[i];
            a[i].y += 1;
        }
        if (!checkCollision()) {
            for (int i = 0; i < 4; i++) {
                field[b[i].y][b[i].x] = color;
            }
            renderWindow->playSoundEffect(LOCK, SFXon);
            nextTetromino();
            holdUsed = false;
        }
        timer = 0;
    }

    // Kiểm tra và xoá các hàng đã hoàn thành
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

    switch (level) {   // Tăng độ khó
        case 1:
            if (linesCleared == 10) {
                level++;
                renderWindow->playSoundEffect(LEVELUP, SFXon);
            }
            else if (linesCleared == 20) {
                level++;
                renderWindow->playSoundEffect(LEVELUP, SFXon);
            }
            break;
        case 2:
            if (linesCleared == 20) {
                level++;
                renderWindow->playSoundEffect(LEVELUP, SFXon);
            }
            break;
        default:
            break;
    }

    linesCleared += linesClearedAtOnce;
    switch (linesClearedAtOnce) {
    case 1:
        score += 40 * (level + 5);
        renderWindow->playSoundEffect(LINE_CLEAR, SFXon);
        break;
    case 2:
        score += 100 * (level + 5);
        renderWindow->playSoundEffect(LINE_CLEAR, SFXon);
        break;
    case 3:
        score += 300 * (level + 5);
        renderWindow->playSoundEffect(LINE_CLEAR, SFXon);
        break;
    case 4:
        score += 1200 * (level + 5);
        renderWindow->playSoundEffect(TETRIS, SFXon);
        break;
    default:
        break;
    }

    dx = 0;
    rotate = false;
    if (level == 1) delay = 0.3f;
    else if (level == 2) delay = 0.2f;
    else if (level == 3) delay = 0.1f;

    updateGhostBlock();
}

// 7-bag randomizer
std::deque<int> queue;

void Game::refillBag() {
    bag = { 0, 1, 2, 3, 4, 5, 6 };
    unsigned seed = std::time(0);
    std::shuffle(bag.begin(), bag.end(), std::default_random_engine(seed));
    bagIndex = 0;
}

void Game::initQueue() {
    queue.clear();
    for (int i = 0; i < 3; i++) {
        if (bagIndex >= bag.size()) refillBag();
        queue.push_back(bag[bagIndex++]);
    }
}

void Game::render() {
    renderWindow->clear();
    renderWindow->renderPlayScreen(score, level, linesCleared, pauseState);
    updateGhostBlock();

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
            dest = { j * TILE_SIZE + BLOCK_RENDER_POS_X, i * TILE_SIZE + BLOCK_RENDER_POS_Y, TILE_SIZE, TILE_SIZE };
            renderWindow->render(tileTexture, &src, &dest);
        }
    }

    // Draw the ghost block
    for (int i = 0; i < 4; i++) {
        src = { color * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE };
        dest = { ghost[i].x * TILE_SIZE + BLOCK_RENDER_POS_X, ghost[i].y * TILE_SIZE + BLOCK_RENDER_POS_Y, TILE_SIZE, TILE_SIZE };
        renderWindow->render(ghostTexture, &src, &dest);
    }

    // Draw the active tetromino
    for (int i = 0; i < 4; i++) {
        src = { color * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE };
        dest = { a[i].x * TILE_SIZE + BLOCK_RENDER_POS_X, a[i].y * TILE_SIZE + BLOCK_RENDER_POS_Y, TILE_SIZE, TILE_SIZE };
        renderWindow->render(tileTexture, &src, &dest);
    }

    renderWindow->display();
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
        else if (a[i].y >= 0 && field[a[i].y][a[i].x]) return false;
    }
    return true;
}

bool Game::checkGhostCollision() {
    for (int i = 0; i < 4; i++) {
        if (ghost[i].x < 0 || ghost[i].x >= N || ghost[i].y >= M) return false;
        else if (ghost[i].y >= 0 && field[ghost[i].y][ghost[i].x]) return false;
    }
    return true;
}

void Game::updateGhostBlock() {
    // Sao chép vị trí của khối hiện tại
    for (int i = 0; i < 4; i++) {
        ghost[i] = a[i];
    }

    // Di chuyển xuống sâu nhất có thể
    bool canMoveDown = true;
    while (canMoveDown) {
        for (int i = 0; i < 4; i++) {
            ghost[i].y++;
        }

        // Xử lý va chạm của ghost
        for (int i = 0; i < 4; i++) {
            if (ghost[i].x < 0 || ghost[i].x >= N || ghost[i].y >= M ||
                (ghost[i].y >= 0 && field[ghost[i].y][ghost[i].x])) {
                canMoveDown = false;
                break;
            }
        }

        // Nếu không thể di chuyển xuống, quay lại ví trí trước đó
        if (!canMoveDown) {
            for (int i = 0; i < 4; i++) {
                ghost[i].y--;
            }
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
    // Kiểm tra xem khối đã đến hàng trên cùng chưa
    for (int j = 0; j < N; j++) {
        if (field[0][j]) {
            gameRunning = false;
            gameState = GAME_OVER;
            renderWindow->playSoundEffect(GAMEOVER, SFXon);
            renderWindow->updateHighScores(score, highScores);
            renderWindow->saveHighScores("highscore.txt", highScores);
            return;
        }
    }

    // Kiểm tra xem khối mới có overlap với các khối cũ không
    for (int i = 0; i < 4; i++) {
        if (a[i].y >= 0 && field[a[i].y][a[i].x]) {
            gameRunning = false;
            gameState = GAME_OVER;
            renderWindow->playSoundEffect(GAMEOVER, SFXon);
            renderWindow->updateHighScores(score, highScores);
            renderWindow->saveHighScores("highscore.txt", highScores);
            return;
        }
    }
}

bool Game::GamePause() {
    gamePausing = true;
    gameMenuRunning = true;
    gameRunning = true;
    gameOverRunning = true;

    SDL_Event event;

    resumeState = NORMAL;
    quitState = NORMAL;

    const SDL_Rect& resumeRect = renderWindow->getButtonResumeRect();
    const SDL_Rect& quitRect = renderWindow->getButtonQuitRect();

    while (gamePausing) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_Point mousePoint = { mx, my };

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                gamePausing = false;
                break;

            case SDL_MOUSEMOTION:
                resumeState = SDL_PointInRect(&mousePoint, &resumeRect) ? HOVER : NORMAL;
                quitState = SDL_PointInRect(&mousePoint, &quitRect) ? HOVER : NORMAL;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (SDL_PointInRect(&mousePoint, &resumeRect)) resumeState = DOWN;
                    if (SDL_PointInRect(&mousePoint, &quitRect)) quitState = DOWN;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (resumeState == DOWN && SDL_PointInRect(&mousePoint, &resumeRect)) {
                        gameState = PLAYING;
                        Mix_ResumeMusic();
                        gamePausing = false;
                    }

                    if (quitState == DOWN && SDL_PointInRect(&mousePoint, &quitRect)) {
                        gameState = MENU;
                        gamePausing = false;
                    }

                    // Reset trạng thái
                    resumeState = NORMAL;
                    quitState = NORMAL;
                }
                break;
            }
        }
        renderWindow->clear();
        renderWindow->renderPlayScreen(score, level, linesCleared, pauseState);
        renderWindow->renderPauseScreen(resumeState, quitState);
        renderWindow->display();
    }


    return gamePausing;
}

bool Game::GameOver() {
    Mix_HaltMusic();
    gameMenuRunning = true;
    gameRunning = true;
    gamePausing = true;

    SDL_Event event;

    homeState = NORMAL;
    replayState = NORMAL;

    const SDL_Rect& homeRect = renderWindow->getButtonHomeRect();
    const SDL_Rect& replayRect = renderWindow->getButtonReplayRect();

    while (gameOverRunning) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_Point mousePoint = { mx, my };

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                gameOverRunning = false;
                break;

            case SDL_MOUSEMOTION:
                homeState = SDL_PointInRect(&mousePoint, &homeRect) ? HOVER : NORMAL;
                replayState = SDL_PointInRect(&mousePoint, &replayRect) ? HOVER : NORMAL;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (SDL_PointInRect(&mousePoint, &homeRect)) homeState = DOWN;
                    if (SDL_PointInRect(&mousePoint, &replayRect)) replayState = DOWN;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (homeState == DOWN && SDL_PointInRect(&mousePoint, &homeRect)) {
                        gameState = MENU;
                        gameOverRunning = false;
                    }

                    if (replayState == DOWN && SDL_PointInRect(&mousePoint, &replayRect)) {
                        gameState = PLAYING;
                        resetGame();
                        renderWindow->playMusic(musicOn);
                        gameOverRunning = false;
                    }

                    // Reset trạng thái
                    homeState = NORMAL;
                    replayState = NORMAL;
                }
                break;
            }
        }
        renderWindow->clear();
        renderWindow->renderPlayScreen(score, level, linesCleared, pauseState);
        renderWindow->renderGameOverScreen(homeState, replayState, highScores);
        renderWindow->display();
    }


    return gameOverRunning;
}

Game::~Game() {
    SDL_DestroyTexture(tileTexture);
    SDL_DestroyTexture(ghostTexture);
    delete renderWindow;
}