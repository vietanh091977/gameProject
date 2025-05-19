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
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
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
}

void RenderWindow::renderHoldBlock(int color) {
    SDL_RenderCopy(renderer, Whole_Block[color], NULL, &getHoldRect(Whole_Block[color]));
}

void RenderWindow::renderNextBlock(int color, int i) {
    SDL_RenderCopy(renderer, Whole_Block[color], NULL, &getNextRect(Whole_Block[color], i));
}

void RenderWindow::SetRect() {
    scoreRect = { SCORE_POS_X, SCORE_POS_Y, SCORE_WIDTH, SCORE_HEIGHT };
    //levelRect = { scoreRect.x, scoreRect.y + 60 ,scoreRect.w, scoreRect.h };
    linesRect = { scoreRect.x, scoreRect.y + 120, scoreRect.w, scoreRect.h };
    matrixRect = { MATRIX_POS_X, MATRIX_POS_Y, MATRIX_WIDTH, MATRIX_HEIGHT };
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

SDL_Rect& RenderWindow::getMatrixRect() {
    return matrixRect;
}

Mix_Chunk* RenderWindow::loadSoundEffect(const std::string& path) {
    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    return chunk;
}

void RenderWindow::playMusic() {
    Mix_PlayMusic(backgroundMusic, -1);
}

void RenderWindow::playSoundEffect(SFXType type) {
    Mix_PlayChannel(-1, Sound[type], 0);
}