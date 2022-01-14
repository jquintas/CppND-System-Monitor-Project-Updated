#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "process.h"
#include "linux_parser.h"
#include <iostream>
#include <thread>

using std::string;
using std::to_string;
using std::vector;

// @.@ DONE: Return this process's ID
// ?? necessary ??
int Process::Pid() { return pid_; }

// @.DONE: Return this process's CPU utilization
float Process::CpuUtilization() { 

// Formula here -> https://stackoverflow.com/a/16736599/9004795
return (LinuxParser::ActiveJiffies(Pid()) / hertz_) / float(UpTimeProcess());
}

// @.@ DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// @.@ FONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// @.@ DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

// @.@ DONE: Return the age of this process (in seconds)
// Formula : uptime - (starttime / Hertz) -> https://stackoverflow.com/a/16736599/9004795
long int Process::UpTimeProcess() { 
    return LinuxParser::UpTime() - (LinuxParser::StartTimeProcess(Pid()) / hertz_); }

// @.@ DONE: Overload the "less than" comparison operator for Process objects
// RAM could be an alternative?
bool Process::operator<(Process const& a) const {
    return a.cpu_utilization_ < cpu_utilization_; 
}