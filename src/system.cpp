#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// @.@ DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// @.@ DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() { 

    auto pids_list = LinuxParser::Pids();
    processes_.clear();
    for (auto pid : pids_list){
        processes_.push_back(Process(pid, sysconf(_SC_CLK_TCK)));
    }
    std::sort(processes_.begin(), processes_.end(),[](Process& p1, Process& p2){return (p1<p2) ;});

    return processes_; 
}

// @.@ DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return string(LinuxParser::Kernel()); }

// @.@ DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// @.@ DONE: Return the operating system name
std::string System::OperatingSystem() { return string(LinuxParser::OperatingSystem()); }

// @.@ DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// @.@ DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// @.@ DONE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }