#pragma once

#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 480;
#define WINDOW_TITLE "Tetris"

class RenderWindow {
public:
    RenderWindow(const char* title, int width, int height);
    ~RenderWindow();
    SDL_Texture* loadTexture(const std::string& filePath);
    void clear();
    void render(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dest);
    void display();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};

#endif
