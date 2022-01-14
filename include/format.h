#ifndef FORMAT_H
#define FORMAT_H

#include <string>
#include <format.h>
#include <vector>

namespace Format {
std::string ElapsedTimeSystem(long times);  // @.@ DONE/NEW: See src/format.cpp -> like htop
std::string ElapsedTimeProcess(long times); // @.@ NEW: avoid messy display in processes
std::vector<long> ConvertTime(long &times); // @.@ NEW: Convert time

};                                    // namespace Format

#endif