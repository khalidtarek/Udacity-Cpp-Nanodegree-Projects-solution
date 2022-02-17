#include <string>

#include "format.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using std::string;
using std::setw;
using std::ostringstream;
using std::setfill;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{ 
    ostringstream formatTime;
    formatTime << setfill('0') << setw(2) << seconds/3600  << ":"  << setfill('0') << setw(2) << (seconds % 3600) / 60 << ":" << setfill('0') << setw(2) << seconds % 60;
    return formatTime.str(); 
}