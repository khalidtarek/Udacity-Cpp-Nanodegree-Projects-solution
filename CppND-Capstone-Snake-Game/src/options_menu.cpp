
#include <thread>
#include <chrono>
#include <iostream>
#include <string>
#include "options_menu.h"
#include "game.h"

using namespace std;

bool OptionsMenu::gameExit = false;
float OptionsMenu::gameLevel;

// intro print snake shape
void OptionsMenu::introScreen()
{
    cout << " ---------------------------------- " << endl;
    cout << "            SNAKE GAME              " << endl;
    cout << " ---------------------------------- " << endl;
    cout << "                .^^.                " << endl;
    cout << "           >--'- -´))               " << endl;
    cout << "                  //                " << endl;
    cout << "                 ((                 " << endl;
    cout << "                  \\                " << endl;
    cout << "                   ))               " << endl;
    cout << "                  //                " << endl;
    cout << "                 ((                 " << endl;
}

////////////////////////////////////////////////////////////////////////////////

bool OptionsMenu::initialize()
{
    bool levelSelectFlag;
    int optionSelect;
    int levelSelect;
    // view user maximum score or points
    string line, key, value;
    ifstream filestream("../score/max_score.txt");
    introScreen();

    // print out options menu
    std::cout << "1.Select gameLevel" << std::endl;
    std::cout << "2.Maximum score" << std::endl;
    std::cout << "3.Exit the game" << std::endl;

    // wait for user selection
    while (!(std::cin >> optionSelect))
    {
        std::cin.clear();

        while (std::cin.get() != '\n')
            continue;
    }

    // switch to user selected option
    switch (optionSelect)
    {
    case 1:
        // print out options menu
        std::cout << " Select your Level:" << std::endl;
        std::cout << "   1. Beginner" << std::endl;
        std::cout << "   2. intermediate" << std::endl;
        std::cout << "   3. advanced" << std::endl;

        // wait for user selection
        while (!(std::cin >> levelSelect))
        {
            std::cin.clear();

            while (std::cin.get() != '\n')
                continue;
        }

        // switch on user selected option
        switch (levelSelect)
        {
        case 1:
            // print user level and update gameLevel variable accordingly
            std::cout << "Playing as Beginner" << std::endl;
            gameLevel = LEVEL_BEGINNER;
            levelSelectFlag = true;
            break;
        case 2:
            // print user level and update gameLevel variable accordingly
            std::cout << "Playing as intermediate" << std::endl;
            gameLevel = LEVEL_INTERMEDIATE;
            levelSelectFlag = true;
            break;
        case 3:
            // print user level and update gameLevel variable accordingly
            std::cout << "Playing as advanced" << std::endl;
            gameLevel = LEVEL_ADVANCED;
            levelSelectFlag = true;
            break;
        default:
            std::cout << "you pressed : " << levelSelect << std::endl;
            std::cout << "FAILED. Please enter correct number!"
                      << "\n\n";
            levelSelectFlag = false;
            break;
        }
        break;
    case 2:
        // Get maximum score from external file
        if (filestream.is_open())
        {
            getline(filestream, line);
            istringstream linestream(line);
            linestream >> key >> value;
        }
        std::cout << "Your maximum score is : " << value << std::endl;
        levelSelectFlag = false;
        break;
    case 3:
        // exit game
        std::cout << "Exiting now!"
                  << "\n\n";
        gameExit = true;
        levelSelectFlag = false;
        break;
    default:
        // handle wrong input case
        std::cout << "you pressed : " << levelSelect << std::endl;
        std::cout << "FAILED. Please enter correct number!"
                  << "\n\n";
        levelSelectFlag = false;
        break;
    }
    return levelSelectFlag;
}
