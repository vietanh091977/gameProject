#pragma once

#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <unordered_map>
#include "defs.h"

enum BlockType {
    T = 1,
    Z = 2,
    S = 3,
    O = 4,
    I = 5,
    L = 6,
    J = 7
};

enum SFXType {
    GAMEOVER = 1,
    HARD_DROP = 2,
    HOLD = 3,
    LINE_CLEAR = 4,
    LOCK = 5,
    MOVE = 6,
    ROTATE = 7
};

class RenderWindow {
public:
    RenderWindow(const char* title, int width, int height);
    ~RenderWindow();
    SDL_Texture* loadTexture(const std::string& filePath);
    SDL_Texture* loadText(const std::string& text, SDL_Color color);
    void clear();
    void SetRect();
    void render(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dest);
    void renderScoreText(int score);
    void renderLinesText(int lines);
    void display();
    void loadAssets();

    SDL_Rect& getMatrixRect();
    SDL_Rect& getHoldRect(SDL_Texture* texture);
    SDL_Rect& getNextRect(SDL_Texture* texture, int i);
    SDL_Rect& getScoreTextRect(SDL_Texture* texture);
    SDL_Rect& getLinesTextRect(SDL_Texture* texture);
    void renderHoldBlock(int color);
    void renderNextBlock(int color, int i);
    
    void playMusic();
    void playSoundEffect(SFXType type);

private:
    std::unordered_map<int, SDL_Texture*> Whole_Block;
    std::unordered_map<int, Mix_Chunk*> Sound;
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Rect scoreRect;
    SDL_Rect linesRect;
    SDL_Rect matrixRect;
    SDL_Rect holdRect;
    SDL_Rect queueRect[3];
    SDL_Rect scoreTextRect;
    SDL_Rect linesTextRect;

    TTF_Font* font;
    Mix_Music* backgroundMusic;
    Mix_Chunk* loadSoundEffect(const std::string& path);
};

#endif
