#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"
#include "options_menu.h"

// new class created to add new feature for bonus food
class BonusFood
{
public:
    int x = 0;
    int y = 0;
    std::vector<int> colorRGB{0xFF, 0xFF, 0XFF};
    void RemainingTimeInSecs(long time) { _remainingTimeInSecs = time; }
    long RemainingTimeInSecs(void) { return _remainingTimeInSecs; }
    void IsBounsFoodReached(bool b) { _isBounsFoodReached = b; }
    bool IsBounsFoodReached(void) { return _isBounsFoodReached; }
    void IsTimeOut(bool b) { _isTimeOut = b; }
    bool IsTimeOut(void) { return _isTimeOut; }

private:
    long _remainingTimeInSecs;
    bool _isBounsFoodReached = false;
    bool _isTimeOut = false;
};

class Renderer
{
public:
    Renderer(const std::size_t screen_width, const std::size_t screen_height,
             const std::size_t grid_width, const std::size_t grid_height);
    ~Renderer();

    void Render(Snake const snake, SDL_Point const &food, BonusFood const &bonusFood, bool startBounsFood);
    void UpdateWindowTitle(int score, int fps, int remainingTime, bool startBounsFood);

private:
    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;

    const std::size_t screen_width;
    const std::size_t screen_height;
    const std::size_t grid_width;
    const std::size_t grid_height;

    OptionsMenu snakeGameMenu;
};

#endif