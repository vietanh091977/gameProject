#include "renderWindow.h"

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
