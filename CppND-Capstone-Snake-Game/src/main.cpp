#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "options_menu.h"

int main()
{
    constexpr std::size_t kFramesPerSecond{60};
    constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
    constexpr std::size_t kScreenWidth{640};
    constexpr std::size_t kScreenHeight{640};
    constexpr std::size_t kGridWidth{32};
    constexpr std::size_t kGridHeight{32};

    OptionsMenu snakeGameMenu;

    while (true)
    {
        while (snakeGameMenu.initialize() == false)
        {
            if (snakeGameMenu.gameExit == true)
            {
                return 0;
            }
            else
            {
                // wait for user input to start play (wait until flag become true to continue)
            }
        }
        std::cout << "Game level " << snakeGameMenu.gameLevel << std::endl;

        Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
        Controller controller;
        Game game(kGridWidth, kGridHeight);
        // set snake speed according to user selected level
        game.SetSnakeSpeed((snakeGameMenu.gameLevel * 0.2));
        game.Run(controller, renderer, kMsPerFrame);
        std::cout << "Game has terminated successfully!\n";
        int score, maxScore;
        score = game.GetScore();
        maxScore = game.GetMaxScore();
        if(score > maxScore)
        {
            maxScore = score;
            game.SetMaxScore(maxScore);
        }
        std::cout << "Maximum Score: " << maxScore << "\n";
        std::cout << "Score: " << score << "\n";
        std::cout << "Size: " << game.GetSize() << "\n";
    }

    return 0;
}