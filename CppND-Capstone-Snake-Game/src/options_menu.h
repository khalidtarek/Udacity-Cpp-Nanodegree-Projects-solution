#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H

#include <iostream>
#include <vector>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#define LEVEL_BEGINNER              1 
#define LEVEL_INTERMEDIATE          2
#define LEVEL_ADVANCED              3

class OptionsMenu
{

public:
    OptionsMenu() { };
    void introScreen();
    bool initialize();
    bool selectGameMode();
    static float gameLevel;
    static bool gameExit;
};

#endif