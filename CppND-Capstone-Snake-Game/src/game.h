#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include <thread>
#include <future>
#include <mutex>
#include <queue>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "options_menu.h"

using namespace std;

// message queue class for bonus food notification
template <class T>
class MessageQueue
{
public:
    void send(T &&messgae);
    T receive();
    bool isEmpty() { return _messageQueue.empty(); };

private:
    std::deque<T> _messageQueue;
    std::condition_variable _condition;
    std::mutex _mutex;
};

// bonus point class for bonus food points
class BounsFoodPoint : public SDL_Point
{
public:
    BounsFoodPoint(){};
    void TimeOutSec(int x) { _timeOutSec = x; }
    int TimeOutSec(void) { return _timeOutSec; }

private:
    int _timeOutSec = 5;
};

// bonus food handling
class BonusFoodHandling
{
public:
    BonusFoodHandling(std::size_t gridWidth, std::size_t gridHeight);
    void BounsFoodUpdate(int bounsFoodX, int bounsFoodY, Snake *snake, std::shared_ptr<MessageQueue<BonusFood>> _bonusFoodQueue);

private:
    std::unique_ptr<BounsFoodPoint> _bounsFoodPtr = std::make_unique<BounsFoodPoint>();
    void PlaceBonusFood(int bounsFoodX, int bounsFoodY, Snake snake);
    //Similar Implementation of Food for generating Random Implementation
    std::random_device dev;
    std::mt19937 engine;
    std::uniform_int_distribution<int> random_w;
    std::uniform_int_distribution<int> random_h;
};

class Game
{
public:
    Game(std::size_t grid_width, std::size_t grid_height);
    void Run(Controller const &controller, Renderer &renderer,
             std::size_t target_frame_duration);
    int GetScore() const;
    int GetSize() const;
    // get maximum score stored in external file
    int GetMaxScore() const;
    // set maximum score in external file
    void SetMaxScore(int score);
    // set snake speed according to user selected level
    void SetSnakeSpeed(float snakeSpeed);

private:
    Snake snake;
    SDL_Point food;

    std::random_device dev;
    std::mt19937 engine;
    std::uniform_int_distribution<int> random_w;
    std::uniform_int_distribution<int> random_h;

    OptionsMenu snakeGameMenu;

    bool _startBounsFood = false;
    BonusFood _bonusFood;
    std::future<void> _bounsFoodfuture;
    std::shared_ptr<MessageQueue<BonusFood>> _bonusFoodQueue = std::make_shared<MessageQueue<BonusFood>>();
    std::size_t _gridWidth;
    std::size_t _gridHeight;

    int score{0};

    void PlaceFood();
    void Update();
    void HandleBounsFood();
};

#endif