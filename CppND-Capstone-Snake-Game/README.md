the project "Snake Game" has chosen and as starter code, the repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). was used

## new features: 
options menu which make control of the game as follows
1-intro with an asci draw as logo for the game
2-input from the user to naivate the options
  1-difficulty selection
  2-show maximum score 
  3-exit option
3-bonus food appear at random positions with count down timer

The deficulity levels manage different speeds and constraints as follows
  1 play as beginner ( low speed and open frame)
  2 play as intermediat ( medium speed with closed frame (green wall added))
  3 play as advanced ( high speed with closed frame (green wall added)) 

<img src="snake_game.gif"/>
---------------------------------------------------------------
## Game explanation:

run ./SnakeGame

- at the begining a logo will appear on the CMD
- a user input will be required 
 press 1 to select the difficulty level
 press 2 to show the maximum score on the CMD
 press 3 to exit 
 
 to select the difficuty level press 1 
 then another input will be required : 
  press 1 to play as beginner ( low speed and open frame)
  press 2 to play as intermediat ( medium speed with closed frame)
  press 3 to play as advanced ( high speed with closed frame) 
  
 after selecting the level , a pop-up window will appear with snake moving by arrows of the keyboard
 eat more food to get longer and get more scores. 
 
 after the head of the snake hits the frame(intermediat or advanced level) or hits the body of the snake
 the game will over.

## Criterias 
1- update the README.MD file with all the required info
2- project in compiliable and runnable
3- the project demonstrates an understanding of C++ functions and control structures
   1- Loops >> while loops have been used in main.c (while loops) and in options_menu.cpp as polling for the user input
   2- Functions >> functions are used in the options_menu.cpp file, and declared in main_menu.h
   3- I/O >> the maximum score file is uesed to read the data from it and store data to it according to current user score (SetMaxScore and GetMaxScore) in game.c
4- The project uses OOP
  1- new class in options_menu.h has been added ( class OptionsMenu) in addition to multiple classes for Bonus food handling BonusFood in render.h and (BounsFoodPoint, BonusFoodHandling and MessageQueue) in game.h
  2- Classes use appropriate access specifiers >> class in options_menu.h has been added ( class OptionsMenu with public)
  3- Classes abstract implementation details from their interfaces function names are well descriped and commented inside added classes.
  4- Classes encapsulate behavior features are groupped into classes accessible by class member fucnctions (OptionsMenu, BounsFoodPoint, BonusFoodHandling and MessageQueue)
  5- Classes follow an appropriate inheritance hierarchy (BounsFoodPoint inherite SDL_Point class) in game.h
  7- Templates generalize functions in the project (MessageQueue) in game.h
5- Memory Management: in HandleBounsFood(), Receive(), Send(), PlaceBonusFood(), HandleBounsFood

6- Concurrency: in game.h and game.cpp file in MessageQueue class and HandleBounsFood() function








## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.


## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
