#include "game.h"
#include <iostream>
#include "SDL.h"
#include <string>

// Implementation of class "MessageQueue" 

template <typename T>
T MessageQueue<T>::receive()
{
    // mutex unique lock
    std::unique_lock<std::mutex> lockMutex(_mutex);

    // pass unique lock to condition variable
    _condition.wait(lockMutex, [this]
                    { return !_messageQueue.empty(); });

    // remove last vector element from queue
    T message = std::move(_messageQueue.back());
    _messageQueue.pop_back();

    // will not be copied due to rvo
    return message;
}

template <typename T>
void MessageQueue<T>::send(T &&message)
{
    // mutex lock
    std::lock_guard<std::mutex> lockMutex(_mutex);
    _messageQueue.clear();
    // add message to queue
    _messageQueue.push_back(std::move(message));

    // notify client after pushing new message into vector
    _condition.notify_one();
}

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      _gridHeight(grid_height),
      _gridWidth(grid_width)
{
    PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration)
{
    Uint32 title_timestamp = SDL_GetTicks();
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    int frame_count = 0;
    bool running = true;

    while (running)
    {
        frame_start = SDL_GetTicks();

        // Input, Update, Render - the main game loop.
        controller.HandleInput(running, snake);
        Update();

        renderer.Render(snake, food, _bonusFood, _startBounsFood);

        frame_end = SDL_GetTicks();

        // Keep track of how long each loop through the input/update/render cycle
        // takes.
        frame_count++;
        frame_duration = frame_end - frame_start;

        // After every second, update the window title.
        if (frame_end - title_timestamp >= 1000)
        {
            renderer.UpdateWindowTitle(score, frame_count, _bonusFood.RemainingTimeInSecs() + 1, _startBounsFood);

            frame_count = 0;
            title_timestamp = frame_end;
        }

        // If the time for this frame is too small (i.e. frame_duration is
        // smaller than the target ms_per_frame), delay the loop to
        // achieve the correct frame rate.
        if (frame_duration < target_frame_duration)
        {
            SDL_Delay(target_frame_duration - frame_duration);
        }
    }
}

void Game::PlaceFood()
{
    int xPosition, yPosition;
    while (true)
    {
        xPosition = random_w(engine);
        yPosition = random_h(engine);
        // Check that the location is not occupied by a snake item before placing
        // food.
        if (!snake.SnakeCell(xPosition, yPosition))
        {
            food.x = xPosition;
            food.y = yPosition;
            return;
        }
    }
}

void Game::Update()
{
    static int BounsFoodCntr = 0;

    if (!snake.alive)
        return;

    snake.Update();

    int new_xPosition = static_cast<int>(snake.head_x);
    int new_yPosition = static_cast<int>(snake.head_y);

    // Check if there's food over here
    if (food.x == new_xPosition && food.y == new_yPosition)
    {
        score++;
        BounsFoodCntr++;
        PlaceFood();
        //check if Bouns Food time started
        if ((BounsFoodCntr >= 5) && (!_startBounsFood))
        {
            BounsFoodCntr = 0;
            // trigger Bouns Food handling 
            HandleBounsFood();
        }
        // Grow snake and increase speed.
        snake.GrowBody();
        snake.speed += 0.02;
    }

    // check that the message queue has data inside 
    if (!_bonusFoodQueue->isEmpty())
    {
        // receive the queued data from the parallel thread and take the required action
        _bonusFood = _bonusFoodQueue->receive();

        if (_bonusFood.IsBounsFoodReached())
        {
            // increase the score three points
            score += 5;

            // Bouns Food Time has ended 
            _startBounsFood = false;
            // Grow snake and increase speed. 
            snake.GrowBody();
            snake.speed += 0.03;
        }
        else if (_bonusFood.IsTimeOut())
        {
            // Bouns Food Time has ended 
            _startBounsFood = false;
        }
        else
        {
            // Bouns Food Time has Started 
            _startBounsFood = true;
        }
    }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

// get maximum score stored in external file
int Game::GetMaxScore() const
{
    string line, key, value;
    ifstream filestream("../score/max_score.txt");
    // Get the number of running processes
    if (filestream.is_open())
    {
        getline(filestream, line);
        istringstream linestream(line);
        linestream >> key >> value;
        
        return std::stoi(value);
    }

    return 0;
}

// set maximum score in external file
void Game::SetMaxScore(int score)
{
    string line;
    ofstream filestream("../score/max_score.txt");
    // Get the number of running processes
    if (filestream.is_open())
    {
        filestream << "Max_score " << score;
    }
}

// set snake speed according to user selected level
void Game::SetSnakeSpeed(float snakeSpeed) { snake.speed = snakeSpeed; }

// method to trigger Bonus Food thread
void Game::HandleBounsFood()
{
    // unique pointer allocating in heap temporary instance of the object BonusFoodHandling
    std::unique_ptr<BonusFoodHandling> bonusFoodPtr = std::make_unique<BonusFoodHandling>(this->_gridWidth, this->_gridHeight);
    // trigget the concurrent function asyncronously 
    _bounsFoodfuture = (std::async(std::launch::async, &BonusFoodHandling::BounsFoodUpdate,
                                   std::move(bonusFoodPtr), this->food.x, this->food.y, &this->snake, this->_bonusFoodQueue));
}

// BonusFoodHandling constructor
BonusFoodHandling::BonusFoodHandling(std::size_t gridWidth, std::size_t gridHeight)
    : engine(dev()), random_w(0, static_cast<int>(gridWidth - 1)),
      random_h(0, static_cast<int>(gridHeight - 1)) {}


// function for the Updating bouns Food
void BonusFoodHandling::BounsFoodUpdate(int bounsFoodX, int bounsFoodY,
                                        Snake *snake, std::shared_ptr<MessageQueue<BonusFood>> _bonusFoodQueue)
{
    BonusFood bounsFood;
    long nowTiming;
    long initTiming = SDL_GetTicks();
    long foodBlinkTimer = SDL_GetTicks();
    long timeDifference = 0;
    bool foodblink = false;

    // place the bouns food
    PlaceBonusFood(bounsFoodX, bounsFoodY, *snake);
    bounsFood.x = _bounsFoodPtr->x;
    bounsFood.y = _bounsFoodPtr->y;

    while (true)
    {
        // check the elabsed time from the starting of the thread 
        nowTiming = SDL_GetTicks();
        timeDifference = nowTiming - initTiming;

        // check if the snake  get the bouns food
        if (snake->SnakeCell(_bounsFoodPtr->x, _bounsFoodPtr->y))
        {
            // update the the related flags and the reamining time
            bounsFood.IsBounsFoodReached(true);
            bounsFood.RemainingTimeInSecs(0);
            bounsFood.IsTimeOut(false);
            break;
        }
        // the normal case : not time out and the snake still doesn't find the Bonus Food
        else if (timeDifference < (_bounsFoodPtr->TimeOutSec()) * 1000)
        {
            // update the the related flags and the reamining time
            bounsFood.IsBounsFoodReached(false);
            bounsFood.RemainingTimeInSecs((((_bounsFoodPtr->TimeOutSec()) * 1000) - timeDifference) / 1000);
            bounsFood.IsTimeOut(false);

            // Change the color of the Bonus Food between two colors each 500ms
            // also change the flag each 500ms
            if ((nowTiming - foodBlinkTimer) > 500)
            {
                foodBlinkTimer = nowTiming;
                foodblink = !foodblink;
            }
            // change the color
            if (foodblink)
                bounsFood.colorRGB = {0, 191, 255};
            else
                bounsFood.colorRGB = {240, 15, 255};
        }
        else // TimeOut :  in case the configured time is elabsed 
        {
            // update the the related flags and the reamining time
            bounsFood.IsBounsFoodReached(false);
            bounsFood.RemainingTimeInSecs(0);
            bounsFood.IsTimeOut(true);
            break;
        }
        // send data in queue after each cycle 
        _bonusFoodQueue->send(std::move(bounsFood));
    }
    // send the final status of data after TimeOut or the snake get the bouns Food
    _bonusFoodQueue->send(std::move(bounsFood));
}

// method to get the new location of the Bonus Food 
void BonusFoodHandling::PlaceBonusFood(int bounsFoodX, int bounsFoodY, Snake snake)
{
    int xPosition, yPosition;
    while (true)
    {

        // get random values for x and y 
        xPosition = random_w(engine);
        yPosition = random_h(engine);

        // Check that the location is not occupied by a snake item before placing bouns food.
        // check that the location is not occupied by the normal food 
        if (!snake.SnakeCell(xPosition, yPosition) && (bounsFoodX != xPosition) && (bounsFoodY != yPosition))
        {
            //update the location
            _bounsFoodPtr->x = xPosition;
            _bounsFoodPtr->y = yPosition;
            return;
        }
    }
}