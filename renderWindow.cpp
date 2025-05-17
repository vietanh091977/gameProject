#include "renderWindow.h"

enum BlockType {
    T = 1,
    Z = 2,
    S = 3,
    O = 4,
    I = 5,
    L = 6,
    J = 7
};

RenderWindow::RenderWindow(const char* title, int width, int height) {
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

RenderWindow::~RenderWindow() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Texture* RenderWindow::loadTexture(const std::string& filePath) {
    return IMG_LoadTexture(renderer, filePath.c_str());
}

void RenderWindow::clear() {
    SDL_RenderClear(renderer);
}

void RenderWindow::render(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dest) {
    SDL_RenderCopy(renderer, texture, src, dest);
}

void RenderWindow::display() {
    SDL_RenderPresent(renderer);
}

void RenderWindow::loadAssets() {
    Whole_Block[T] = loadTexture("images/whole/T.png");
    Whole_Block[Z] = loadTexture("images/whole/Z.png");
    Whole_Block[S] = loadTexture("images/whole/S.png");
    Whole_Block[O] = loadTexture("images/whole/O.png");
    Whole_Block[I] = loadTexture("images/whole/I.png");
    Whole_Block[J] = loadTexture("images/whole/J.png");
    Whole_Block[L] = loadTexture("images/whole/L.png");
}

void RenderWindow::renderHoldBlock(int color) {
    SDL_RenderCopy(renderer, Whole_Block[color], NULL, &getHoldRect(Whole_Block[color]));
}

void RenderWindow::renderNextBlock(int color, int i) {
    SDL_RenderCopy(renderer, Whole_Block[color], NULL, &getNextRect(Whole_Block[color], i));
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
