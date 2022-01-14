#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// @.@ NEW: desired keys

const std::string kMemTotalKey{"MemTotal:"};
const std::string kMemFreeKey{"MemFree:"};
const std::string kProcsRunningKey{"procs_running"};
const std::string kProcessesKey{"processes"};
const std::string kCpuKey{"cpu"};
const std::string kUidKey{"Uid:"};
const std::string kMemProcessKey{"VmSize:"};



// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
// This can be used to get the values of CpuUtilization, kind of nice
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};

// @.@ NEW: For avoiding the classical for loop, prefered this way. Maybe there is another way to do this.
constexpr std::initializer_list<CPUStates> all_CPUStates = {
  kUser_,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
  };


std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
// Changed UpTime to StartTimeProcess, more clear now
long int StartTimeProcess(int pid);

/*
Helper functions
New templates for getting the values, doesn't make sense repeat the code in the cpp file
*/
template <typename T> T getParsedValueWithKey(std::string const &desired_key, std::string const &filename);
template <typename T> T getParsedValueWithKey(std::string const &desired_key);
template <typename T> T getParsedValue(std::string const &filename);

// helper function to read proc/pid/status
std::vector<std::string> getParsePidStatus(std::string const &pid);

};  // namespace LinuxParser

#endif