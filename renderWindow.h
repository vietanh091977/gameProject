#pragma once

#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>
#include "defs.h"

enum TextureType {
    BACKGROUND = 100,
    MATRIX = 101,
    LOGO = 1,
    PAUSE_FRAME = 2,
    GAMEOVER_FRAME = 3,
    ICON_HOME = 4,
    ICON_REPLAY = 5,
    HIGH_SCORE = 6,
    L1 = 1,
    L2 = 2,
    L3 = 3,
    WITH_TEXT = 100,
    RESUME = 15,
    QUIT = 16,
    PLAY = 17,
    SCORE = 18,
    LINES = 19,
    GREEN = 20,
    BLACK = 21,
    MINI = 22,
    PAUSE = 23,
};

enum ButtonState {
    NORMAL = 100,
    HOVER = 200,
    DOWN = 300
};

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
    ROTATE = 7,
    TETRIS = 8,
    LEVELUP = 9
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

    SDL_Rect& getLogoRect();
    SDL_Rect& getMatrixRect();
    SDL_Rect& getPauseFrameRect();
    SDL_Rect& getGameOverFrameRect();
    SDL_Rect& getIconHomeRect();
    SDL_Rect& getIconReplayRect();
    SDL_Rect& getHighScoreRect();

    SDL_Rect& getButtonPlayRect();
    SDL_Rect& getButtonLevelRect();
    SDL_Rect& getButtonResumeRect();
    SDL_Rect& getButtonQuitRect();
    SDL_Rect& getButtonPauseRect();
    SDL_Rect& getButtonHomeRect();
    SDL_Rect& getButtonReplayRect();

    SDL_Rect& getPlayTextRect(SDL_Texture* texture);
    SDL_Rect& getLevelTextRect(SDL_Texture* texture);
    SDL_Rect& getResumeTextRect(SDL_Texture* texture);
    SDL_Rect& getQuitTextRect(SDL_Texture* texture);
    SDL_Rect& getLevelNumberTextRect(SDL_Texture* texture);

    SDL_Rect& getHoldRect(SDL_Texture* texture);
    SDL_Rect& getNextRect(SDL_Texture* texture, int i);
    SDL_Rect& getScoreTextRect(SDL_Texture* texture);
    SDL_Rect& getLinesTextRect(SDL_Texture* texture);
    SDL_Rect& getMenuHighScoreTextRect(SDL_Texture* texture, int i);
    SDL_Rect& getGameOverHighScoreTextRect(SDL_Texture* texture, int i);

    void renderHoldBlock(int color);
    void renderNextBlock(int color, int i);
    
    void playMusic(bool musicOn);
    void playSoundEffect(SFXType type, bool SFXon);

    void loadHighScores(const std::string& filename, std::vector <int>& highScores);
    void saveHighScores(const std::string& filename, std::vector <int>& highScore);
    void updateHighScores(int newScore, std::vector<int>& highScores);

    void renderMenuScreen(ButtonState playState, ButtonState levelState, int level, std::vector <int>& highScores);
    void renderPlayScreen(int score, int level, int lines, ButtonState pauseState);
    void renderPauseScreen(ButtonState resumeState, ButtonState quitState);
    void renderGameOverScreen(ButtonState homeState, ButtonState replayState, std::vector <int>& highScores);

private:
    std::unordered_map<int, SDL_Texture*> Texture;
    std::unordered_map<int, SDL_Texture*> Text;
    std::unordered_map<int, SDL_Texture*> Whole_Block;
    std::unordered_map<int, SDL_Texture*> high_score;
    std::unordered_map<int, Mix_Chunk*> Sound;
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Rect mainLogoRect;
    SDL_Rect matrixRect;
    SDL_Rect pauseFrameRect;
    SDL_Rect gameOverFrameRect;
    SDL_Rect iconHomeRect;
    SDL_Rect iconReplayRect;
    SDL_Rect highScoreFrameRect;

    SDL_Rect buttonPlayRect;
    SDL_Rect buttonLevelRect;
    SDL_Rect buttonResumeRect;
    SDL_Rect buttonQuitRect;
    SDL_Rect buttonPauseRect;
    SDL_Rect buttonHomeRect;
    SDL_Rect buttonReplayRect;

    SDL_Rect playTextRect;
    SDL_Rect levelTextRect;
    SDL_Rect resumeTextRect;
    SDL_Rect quitTextRect;
    SDL_Rect scoreTextRect;
    SDL_Rect levelNumberTextRect;
    SDL_Rect linesTextRect;
    SDL_Rect highScoreTextRect[5];

    SDL_Rect scoreRect;
    SDL_Rect levelRect;
    SDL_Rect linesRect;
    SDL_Rect holdRect;
    SDL_Rect queueRect[3];
    SDL_Rect menuHighScoreTextRect[5];
    SDL_Rect gameOverHighScoreTextRect[5];

    TTF_Font* font;
    Mix_Music* backgroundMusic;
    Mix_Chunk* loadSoundEffect(const std::string& path);
};

#endif
