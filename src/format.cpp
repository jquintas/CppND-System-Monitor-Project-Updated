#include <string>
#include <time.h>

#include "format.h"

using std::string;

// @DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: DAYS, HH:MM:SS like htop

string Format::ElapsedTimeSystem(long times) { 
    
    // Convert to dd HH MM SS -> vector[dd,HH,MM,SS]
    std::vector<long> time = ConvertTime(times);

    // *.* Build output string, did ternary operator to avoid the messy display in ncurses 
    if (time[0] >= 1)
        return std::to_string(time[0]) + " days, " + ((time[1]> 10) ? std::to_string(time[1]) : "0" + std::to_string(time[1])) + ":" + 
      ((time[2]>= 10) ? std::to_string(time[2]) : "0" +std::to_string(time[2])) + ":" + 
      ((time[3]>= 10) ? std::to_string(time[3]) : "0" + std::to_string(time[3]));
    else
        return ((time[1]> 10) ? std::to_string(time[1]) : "0" +std::to_string(time[1])) + ":" + 
      ((time[2]>= 10) ? std::to_string(time[2]) : "0" +std::to_string(time[2])) + ":" + 
      ((time[3]>= 10) ? std::to_string(time[3]) : "0" +std::to_string(time[3]));  
 }

// @.@ NEW: elapsed time for processes
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS  
 string Format::ElapsedTimeProcess(long times) { 
    // Convert to dd HH MM SS -> vector[HH,MM,SS]
    std::vector<long> time = ConvertTime(times);

    // *.* Build output string, did ternary operator to avoid the messy display in ncurses, 
    return ((time[1]> 10) ? std::to_string(time[1]) : "0" +std::to_string(time[1])) + ":" + 
      ((time[2]>= 10) ? std::to_string(time[2]) : "0" +std::to_string(time[2])) + ":" + 
      ((time[3]>= 10) ? std::to_string(time[3]) : "0" +std::to_string(time[3])); 
 }

std::vector<long> Format::ConvertTime(long &times){
    

    long minutes = times / 60;
    long hours = minutes / 60;
    long days = hours / 24; 
    long seconds = int(times % 60);
    minutes = int(minutes % 60);
    return std::vector<long>{days, hours, minutes, seconds};

}
