#include <string>
#include <iomanip>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    std::stringstream ETime;

    const int secinMin = 60;
    const int secinhour = 3600;

    const int hours_ = seconds/secinhour;
    const int mins_ = (seconds - hours_*secinhour)/secinMin;
    const int seconds_ = (seconds-hours_*secinhour-mins_*secinMin);

    ETime << std::setfill('0')<<std::setw(2)<<hours_<<":";
    ETime << std::setfill('0')<<std::setw(2)<<mins_<<":";
    ETime << std::setfill('0')<<std::setw(2)<<seconds_;

    return ETime.str();
}