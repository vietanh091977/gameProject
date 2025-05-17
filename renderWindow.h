#pragma once

#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <unordered_map>
#include "defs.h"

class RenderWindow {
public:
    RenderWindow(const char* title, int width, int height);
    ~RenderWindow();
    SDL_Texture* loadTexture(const std::string& filePath);
    void clear();
    void render(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dest);
    void display();
    void loadAssets();

    SDL_Rect& getHoldRect(SDL_Texture* texture);
    SDL_Rect& getNextRect(SDL_Texture* texture, int i);
    void renderHoldBlock(int color);
    void renderNextBlock(int color, int i);

private:
    std::unordered_map<int, SDL_Texture*> Whole_Block;
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Rect holdRect;
    SDL_Rect queueRect[3];
};

#endif
