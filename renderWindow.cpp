#include "renderWindow.h"

RenderWindow::RenderWindow(const char* title, int width, int height) {
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    SDL_Init(SDL_INIT_EVERYTHING);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    font = TTF_OpenFont("assets/font/MuseoSans500.otf", 24);
    backgroundMusic = Mix_LoadMUS("assets/sfx/backgroundmusic.mp3");
}

RenderWindow::~RenderWindow() {
    for (auto& pair :Texture) {
        SDL_DestroyTexture(pair.second);
    }

    for (auto& pair : Text) {
        SDL_DestroyTexture(pair.second);
    }

    for (auto& pair : Whole_Block) {
        SDL_DestroyTexture(pair.second);
    }

    for (auto& pair : high_score) {
        SDL_DestroyTexture(pair.second);
    }

    for (auto& pair : Sound) {
        Mix_FreeChunk(pair.second);
    }

    Sound.clear();
    high_score.clear();
    Whole_Block.clear();
    Texture.clear();
    Text.clear();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeMusic(backgroundMusic);
    TTF_CloseFont(font);
    SDL_Quit();
    TTF_Quit();
    Mix_CloseAudio();
}

SDL_Texture* RenderWindow::loadTexture(const std::string& filePath) {
    return IMG_LoadTexture(renderer, filePath.c_str());
}

SDL_Texture* RenderWindow::loadText(const std::string& text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void RenderWindow::clear() {
    SDL_RenderClear(renderer);
}

void RenderWindow::render(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dest) {
    SDL_RenderCopy(renderer, texture, src, dest);
}

void RenderWindow::renderScoreText(int score) {
    std::string scoreText = std::to_string(score);
    SDL_Texture* scoreTexture = loadText(scoreText, { 255, 255, 255, 255 });
    SDL_RenderCopy(renderer, scoreTexture, NULL, &getScoreTextRect(scoreTexture));
}

void RenderWindow::renderLinesText(int lines) {
    std::string linesText = std::to_string(lines);
    SDL_Texture* linesTexture = loadText(linesText, { 255, 255, 255, 255 });
    SDL_RenderCopy(renderer, linesTexture, NULL, &getLinesTextRect(linesTexture));
}

void RenderWindow::display() {
    SDL_RenderPresent(renderer);
}

void RenderWindow::loadAssets() {
    Texture[BACKGROUND]     = loadTexture("assets/images/background.png");
    Texture[MATRIX]         = loadTexture("assets/images/matrix.png");
    Texture[LOGO]           = loadTexture("assets/images/logo.png");
    Texture[PAUSE_FRAME]    = loadTexture("assets/images/pause-frame.png");
    Texture[GAMEOVER_FRAME] = loadTexture("assets/images/gameover-frame.png");
    Texture[ICON_HOME]      = loadTexture("assets/images/icon-home.png");
    Texture[ICON_REPLAY]    = loadTexture("assets/images/icon-replay.png");
    Texture[HIGH_SCORE]     = loadTexture("assets/images/highscore-frame.png");
    Texture[GREEN | NORMAL] = loadTexture("assets/images/button/normal/button-green.png");
    Texture[GREEN | HOVER]  = loadTexture("assets/images/button/hover/button-green.png");
    Texture[GREEN | DOWN]   = loadTexture("assets/images/button/down/button-green.png");
    Texture[BLACK | NORMAL] = loadTexture("assets/images/button/normal/button-black.png");
    Texture[BLACK | HOVER]  = loadTexture("assets/images/button/hover/button-black.png");
    Texture[BLACK | DOWN]   = loadTexture("assets/images/button/down/button-black.png");
    Texture[MINI | NORMAL]  = loadTexture("assets/images/button/normal/button-mini.png");
    Texture[MINI | HOVER]   = loadTexture("assets/images/button/hover/button-mini.png");
    Texture[MINI | DOWN]    = loadTexture("assets/images/button/down/button-mini.png");
    Texture[PAUSE | NORMAL] = loadTexture("assets/images/button/normal/button-pause.png");
    Texture[PAUSE | HOVER]  = loadTexture("assets/images/button/hover/button-pause.png");
    Texture[PAUSE | DOWN]   = loadTexture("assets/images/button/down/button-pause.png");

    Whole_Block[T] = loadTexture("assets/images/whole/T.png");
    Whole_Block[Z] = loadTexture("assets/images/whole/Z.png");
    Whole_Block[S] = loadTexture("assets/images/whole/S.png");
    Whole_Block[O] = loadTexture("assets/images/whole/O.png");
    Whole_Block[I] = loadTexture("assets/images/whole/I.png");
    Whole_Block[J] = loadTexture("assets/images/whole/J.png");
    Whole_Block[L] = loadTexture("assets/images/whole/L.png");

    Sound[GAMEOVER]   = loadSoundEffect("assets/sfx/gameover.mp3");
    Sound[HARD_DROP]  = loadSoundEffect("assets/sfx/harddrop.mp3");
    Sound[HOLD]       = loadSoundEffect("assets/sfx/hold.mp3");
    Sound[LINE_CLEAR] = loadSoundEffect("assets/sfx/lineclear.mp3");
    Sound[LOCK]       = loadSoundEffect("assets/sfx/lock.mp3");
    Sound[MOVE]       = loadSoundEffect("assets/sfx/move.mp3");
    Sound[ROTATE]     = loadSoundEffect("assets/sfx/rotate.mp3");
    Sound[TETRIS]     = loadSoundEffect("assets/sfx/tetris.mp3");
    Sound[LEVELUP]    = loadSoundEffect("assets/sfx/levelup.mp3");

    SDL_Color color = { 255, 255, 255, 255 };
    Text[PLAY]           = loadText("PLAY", color);
    Text[L1 | WITH_TEXT] = loadText("LEVEL 1", color);
    Text[L2 | WITH_TEXT] = loadText("LEVEL 2", color);
    Text[L3 | WITH_TEXT] = loadText("LEVEL 3", color);
    Text[RESUME]         = loadText("RESUME", color);
    Text[QUIT]           = loadText("QUIT", color);
    Text[SCORE]          = loadText("0", color);
    Text[L1]             = loadText("1", color);
    Text[L2]             = loadText("2", color);
    Text[L3]             = loadText("3", color);
    Text[LINES]          = loadText("0", color);
}

void RenderWindow::renderHoldBlock(int color) {
    SDL_RenderCopy(renderer, Whole_Block[color], NULL, &getHoldRect(Whole_Block[color]));
}

void RenderWindow::renderNextBlock(int color, int i) {
    SDL_RenderCopy(renderer, Whole_Block[color], NULL, &getNextRect(Whole_Block[color], i));
}

void RenderWindow::SetRect() {

    for (int i = 0; i < 5; i++) {
        menuHighScoreTextRect[i] = { MENU_LINE_POS_X, MENU_LINE_POS_Y + i * LINE_HEIGHT, LINE_WIDTH, LINE_HEIGHT };
        gameOverHighScoreTextRect[i] = { GAMEOVER_LINE_POS_X, GAMEOVER_LINE_POS_Y + i * LINE_HEIGHT, LINE_WIDTH, LINE_HEIGHT };
    }
    mainLogoRect = { LOGO_POS_X, LOGO_POS_Y, LOGO_WIDTH, LOGO_HEIGHT };
    matrixRect = { MATRIX_POS_X, MATRIX_POS_Y, MATRIX_WIDTH, MATRIX_HEIGHT };
    pauseFrameRect = { PAUSED_POS_X, PAUSED_POS_Y, PAUSED_WIDTH, PAUSED_HEIGHT };
    gameOverFrameRect = { GAME_OVER_POS_X, GAME_OVER_POS_Y, GAME_OVER_WIDTH, GAME_OVER_HEIGHT };

    buttonPlayRect = { BUTTON_PLAY_POS_X, BUTTON_PLAY_POS_Y, BUTTON_PLAY_WIDTH, BUTTON_PLAY_HEIGHT };
    buttonLevelRect = { buttonPlayRect.x, buttonPlayRect.y + 60 + TILE_SIZE / 2, BUTTON_PLAY_WIDTH, BUTTON_PLAY_HEIGHT };
    buttonResumeRect = { BUTTON_RESUME_POS_X, BUTTON_RESUME_POS_Y, BUTTON_RESUME_WIDTH, BUTTON_RESUME_HEIGHT };
    buttonQuitRect = { buttonResumeRect.x, buttonResumeRect.y + BUTTON_RESUME_HEIGHT + TILE_SIZE, buttonResumeRect.w, buttonResumeRect.h };
    buttonPauseRect = { BUTTON_PAUSE_POS_X, BUTTON_PAUSE_POS_Y, BUTTON_PAUSE_WIDTH, BUTTON_PAUSE_HEIGHT };
    buttonHomeRect = { BUTTON_HOME_POS_X, BUTTON_HOME_POS_Y, BUTTON_HOME_SIZE, BUTTON_HOME_SIZE };
    buttonReplayRect = { buttonHomeRect.x + 80, buttonHomeRect.y, buttonHomeRect.w, buttonHomeRect.h };

    iconHomeRect = { buttonHomeRect.x + 5, buttonHomeRect.y + 5, 30, 30 };
    iconReplayRect = { buttonReplayRect.x + 5, buttonReplayRect.y + 5, 30, 30 };
    highScoreFrameRect = { HIGH_SCORE_POS_X, HIGH_SCORE_POS_Y, HIGH_SCORE_WIDTH, HIGH_SCORE_HEIGHT };

    scoreRect = { SCORE_POS_X, SCORE_POS_Y, SCORE_WIDTH, SCORE_HEIGHT };
    levelRect = { scoreRect.x, scoreRect.y + 60 ,scoreRect.w, scoreRect.h };
    linesRect = { scoreRect.x, scoreRect.y + 120, scoreRect.w, scoreRect.h };
}

SDL_Rect& RenderWindow::getLogoRect() {
    return mainLogoRect;
}

SDL_Rect& RenderWindow::getMatrixRect() {
    return matrixRect;
}

SDL_Rect& RenderWindow::getPauseFrameRect() {
    return pauseFrameRect;
}

SDL_Rect& RenderWindow::getGameOverFrameRect() {
    return gameOverFrameRect;
}

SDL_Rect& RenderWindow::getButtonPlayRect() {
    return buttonPlayRect;
}

SDL_Rect& RenderWindow::getButtonLevelRect() {
    return buttonLevelRect;
}

SDL_Rect& RenderWindow::getButtonResumeRect() {
    return buttonResumeRect;
}

SDL_Rect& RenderWindow::getButtonQuitRect() {
    return buttonQuitRect;
}

SDL_Rect& RenderWindow::getButtonPauseRect() {
    return buttonPauseRect;
}

SDL_Rect& RenderWindow::getButtonHomeRect() {
    return buttonHomeRect;
}

SDL_Rect& RenderWindow::getButtonReplayRect() {
    return buttonReplayRect;
}

SDL_Rect& RenderWindow::getIconHomeRect() {
    return iconHomeRect;
}

SDL_Rect& ::RenderWindow::getIconReplayRect() {
    return iconReplayRect;
}

SDL_Rect& RenderWindow::getHighScoreRect() {
    return highScoreFrameRect;
}

SDL_Rect& RenderWindow::getPlayTextRect(SDL_Texture* texture) {
    int w = 0, h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    int x = buttonPlayRect.x + (buttonPlayRect.w - w) / 2;
    int y = buttonPlayRect.y + (buttonPlayRect.h - h) / 2;
    playTextRect = { x, y, w, h };
    return playTextRect;
}

SDL_Rect& RenderWindow::getLevelTextRect(SDL_Texture* texture) {
    int w = 0, h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    int x = buttonLevelRect.x + (buttonLevelRect.w - w) / 2;
    int y = buttonLevelRect.y + (buttonLevelRect.h - h) / 2;
    levelTextRect = { x, y, w, h };
    return levelTextRect;
}

SDL_Rect& RenderWindow::getResumeTextRect(SDL_Texture* texture) {
    int w = 0, h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    int x = buttonResumeRect.x + (buttonResumeRect.w - w) / 2;
    int y = buttonResumeRect.y + (buttonResumeRect.h - h) / 2;
    resumeTextRect = { x, y, w, h };
    return resumeTextRect;
}

SDL_Rect& RenderWindow::getQuitTextRect(SDL_Texture* texture) {
    int w = 0, h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    int x = buttonQuitRect.x + (buttonQuitRect.w - w) / 2;
    int y = buttonQuitRect.y + (buttonQuitRect.h - h) / 2;
    quitTextRect = { x, y, w, h };
    return quitTextRect;
}

SDL_Rect& RenderWindow::getLevelNumberTextRect(SDL_Texture* texture) {
    int w = 0, h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    int x = levelRect.x + (levelRect.w - w) / 2;
    int y = levelRect.y + (levelRect.h - h) / 2;
    levelNumberTextRect = { x, y, w, h };
    return levelNumberTextRect;
}

SDL_Rect& RenderWindow::getHoldRect(SDL_Texture* texture) {
    int w = 0, h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    int x = HOLD_POS_X + (BLOCK_WIDTH - w) / 2;
    int y = HOLD_POS_Y + (BLOCK_HEIGHT - h) / 2;

    holdRect = { x, y, w, h };
    return holdRect;
}

SDL_Rect& RenderWindow::getNextRect(SDL_Texture* texture, int i) {
    int w = 0, h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    int x = QUEUE_POS_X + (BLOCK_WIDTH - w) / 2;
    int y = QUEUE_POS_Y + (i * BLOCK_HEIGHT) + (BLOCK_HEIGHT - h) / 2;

    queueRect[i] = { x, y, w, h };
    return queueRect[i];
}

SDL_Rect& RenderWindow::getScoreTextRect(SDL_Texture* texture) {
    int w = 0, h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    int x = scoreRect.x + (scoreRect.w - w) / 2;
    int y = scoreRect.y + (scoreRect.h - h) / 2;
    scoreTextRect = { x, y, w, h };
    return scoreTextRect;
}

SDL_Rect& RenderWindow::getLinesTextRect(SDL_Texture* texture) {
    int w = 0, h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    int x = linesRect.x + (linesRect.w - w) / 2;
    int y = linesRect.y + (linesRect.h - h) / 2;
    linesTextRect = { x, y, w, h };
    return linesTextRect;
}

SDL_Rect& RenderWindow::getMenuHighScoreTextRect(SDL_Texture* texture, int i) {
    int w = 0, h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    int x = menuHighScoreTextRect[i].x + (menuHighScoreTextRect[i].w - w) / 2;
    int y = menuHighScoreTextRect[i].y + (menuHighScoreTextRect[i].h - h) / 2;
    highScoreTextRect[i] = { x, y, w, h };
    return highScoreTextRect[i];
}

SDL_Rect& RenderWindow::getGameOverHighScoreTextRect(SDL_Texture* texture, int i) {
    int w = 0, h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    int x = gameOverHighScoreTextRect[i].x + (gameOverHighScoreTextRect[i].w - w) / 2;
    int y = gameOverHighScoreTextRect[i].y + (gameOverHighScoreTextRect[i].h - h) / 2;
    highScoreTextRect[i] = { x, y, w, h };
    return highScoreTextRect[i];
}

Mix_Chunk* RenderWindow::loadSoundEffect(const std::string& path) {
    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    return chunk;
}

void RenderWindow::playMusic(bool musicOn) {
    if (musicOn) {
        Mix_PlayMusic(backgroundMusic, -1);
    }
}

void RenderWindow::playSoundEffect(SFXType type, bool SFXon) {
    if (SFXon) {
        Mix_PlayChannel(-1, Sound[type], 0);
    }
}

void RenderWindow::loadHighScores(const std::string& filename, std::vector <int>& highScores) {
    std::ifstream file(filename);
    highScores.clear();
    if (file.is_open()) {
        int score;
        while (file >> score) {
            highScores.push_back(score);
        }
        file.close();
    }
}

void RenderWindow::saveHighScores(const std::string& filename, std::vector <int>& highScores) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (int score : highScores) {
            file << score << "\n";
        }
        file.close();
    }
}

void RenderWindow::updateHighScores(int newScore, std::vector<int>& highScores) {
    highScores.push_back(newScore);
    std::sort(highScores.rbegin(), highScores.rend());
    if (highScores.size() > 5) {
        highScores.resize(5);
    }
}

void RenderWindow::renderMenuScreen(ButtonState playState, ButtonState levelState, int level, std::vector <int>& highScores) {
    clear();

    SDL_RenderCopy(renderer, Texture[BACKGROUND], NULL, NULL);
    SDL_RenderCopy(renderer, Texture[LOGO], NULL, &getLogoRect());
    SDL_RenderCopy(renderer, Texture[GREEN | playState], NULL, &getButtonPlayRect());
    SDL_RenderCopy(renderer, Texture[BLACK | levelState], NULL, &getButtonLevelRect());
    SDL_RenderCopy(renderer, Texture[HIGH_SCORE], NULL, &getHighScoreRect());

    SDL_RenderCopy(renderer, Text[PLAY], NULL, &getPlayTextRect(Text[PLAY]));
    SDL_RenderCopy(renderer, Text[level | WITH_TEXT], NULL, &getLevelTextRect(Text[level | WITH_TEXT]));

    for (int i = 0; i < 5; i++) {
        std::string highscoreText = std::to_string(highScores[i]);
        high_score[i] = loadText(highscoreText, { 255, 255, 255, 255 });
        SDL_RenderCopy(renderer, high_score[i], NULL, &getMenuHighScoreTextRect(high_score[i], i));
    }

    display();
}

void RenderWindow::renderPlayScreen(int score, int level, int lines, ButtonState pauseState) {
    SDL_RenderCopy(renderer, Texture[BACKGROUND], NULL, NULL);
    SDL_RenderCopy(renderer, Texture[MATRIX], NULL, &getMatrixRect());
    SDL_RenderCopy(renderer, Texture[PAUSE | pauseState], NULL, &getButtonPauseRect());

    SDL_RenderCopy(renderer, Text[level], NULL, &getLevelNumberTextRect(Text[level]));

    renderScoreText(score);
    renderLinesText(lines);
}

void RenderWindow::renderPauseScreen(ButtonState resumeState, ButtonState quitState) {
    // Vẽ một lớp đen mờ
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);  // 128 là độ mờ
    SDL_Rect screen_size = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderFillRect(renderer, &screen_size);

    SDL_RenderCopy(renderer, Texture[PAUSE_FRAME], NULL, &getPauseFrameRect());
    SDL_RenderCopy(renderer, Texture[BLACK | quitState], NULL, &getButtonQuitRect());
    SDL_RenderCopy(renderer, Texture[GREEN | resumeState], NULL, &getButtonResumeRect());
    SDL_RenderCopy(renderer, Text[QUIT], NULL, &getQuitTextRect(Text[QUIT]));
    SDL_RenderCopy(renderer, Text[RESUME], NULL, &getResumeTextRect(Text[RESUME]));
}

void RenderWindow::renderGameOverScreen(ButtonState homeState, ButtonState replayState, std::vector <int>& highScores) {
    // Vẽ một lớp đen mờ
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);  // 128 là độ mờ
    SDL_Rect screen_size = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderFillRect(renderer, &screen_size);

    SDL_RenderCopy(renderer, Texture[GAMEOVER_FRAME], NULL, &getGameOverFrameRect());
    SDL_RenderCopy(renderer, Texture[MINI | homeState], NULL, &getButtonHomeRect());
    SDL_RenderCopy(renderer, Texture[MINI | replayState], NULL, &getButtonReplayRect());
    SDL_RenderCopy(renderer, Texture[ICON_HOME], NULL, &getIconHomeRect());
    SDL_RenderCopy(renderer, Texture[ICON_REPLAY], NULL, &getIconReplayRect());

    for (int i = 0; i < 5; i++) {
        std::string highscoreText = std::to_string(highScores[i]);
        high_score[i] = loadText(highscoreText, { 255, 255, 255, 255 });
        SDL_RenderCopy(renderer, high_score[i], NULL, &getGameOverHighScoreTextRect(high_score[i], i));
    }  
}