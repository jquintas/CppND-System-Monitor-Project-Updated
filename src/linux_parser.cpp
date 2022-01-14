#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  // Added dummy string, this was not reading the correct value
  string os, dummy, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> dummy >>  kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.emplace_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// @.@ DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  
  // *.* get total memory of the system
  float mem_total = LinuxParser::getParsedValueWithKey<float>(LinuxParser::kMemTotalKey, LinuxParser::kMeminfoFilename);
  // *.* Get free memory
  float mem_free = LinuxParser::getParsedValueWithKey<float>(LinuxParser::kMemFreeKey, LinuxParser::kMeminfoFilename);
  
  // *.* return the percentage of total used memory. Used the formula from https://stackoverflow.com/a/41251290
  return (mem_total - mem_free) / mem_total; 
}

// @.@ DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  return LinuxParser::getParsedValue<long>(LinuxParser::kUptimeFilename);
}

// @.@ Done: Read and return the number of jiffies for the system. This shall be used in Processor::Utilization()
long LinuxParser::Jiffies() { 
  auto cpu = LinuxParser::CpuUtilization();
  long total_time{0};
  for (auto key : LinuxParser::all_CPUStates){
    if (key == LinuxParser::CPUStates::kGuest_ || key == LinuxParser::CPUStates::kGuestNice_)
      total_time -= std::stol(cpu[key]); // Should remove the guest? Anyway normally it will be 0
    else  
      total_time += std::stol(cpu[key]); 
  }
  return total_time; 
}

// @.@ DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  std::vector<std::string> pid_status = getParsePidStatus(to_string(pid));

  /* Where:
  #13 utime - CPU time spent in user code, measured in clock ticks
  #14 stime - CPU time spent in kernel code, measured in clock ticks
  #15 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
  #16 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
  from: https://stackoverflow.com/a/16736599
  */
  long total_time_process = std::stol(pid_status[13]) + std::stol(pid_status[14]) + std::stol(pid_status[15]) + std::stol(pid_status[16]);
    return total_time_process; 
}

// @.@ Done: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  return LinuxParser::Jiffies() - LinuxParser::IdleJiffies(); 
}

// @.@ Done: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  auto cpu = LinuxParser::CpuUtilization();
  return std::stol(cpu[LinuxParser::CPUStates::kIdle_]) + std::stol(cpu[LinuxParser::CPUStates::kIOwait_]); }

// @.@ Done: Read and return CPU utilization
//? why I want this? Save the values in a vector of strings? Could this be a double and change everything else to double? are long
// and float really necessary??
vector<std::string> LinuxParser::CpuUtilization() { 
  auto parsed_cpu_values =   LinuxParser::getParsedValueWithKey<std::vector<std::string>>(LinuxParser::kCpuKey);
  return parsed_cpu_values; 
}

// @.@ DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {   
  return LinuxParser::getParsedValueWithKey<int>(LinuxParser::kProcessesKey, LinuxParser::kStatFilename);
}
// @.@ DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  return LinuxParser::getParsedValueWithKey<int>(LinuxParser::kProcsRunningKey, LinuxParser::kStatFilename);
}

// @.@ DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  return LinuxParser::getParsedValue<string>(std::to_string(pid) + LinuxParser::kCmdlineFilename); 
}

// @.@ DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  float ram = LinuxParser::getParsedValueWithKey<float>(LinuxParser::kMemProcessKey, std::to_string(pid) + LinuxParser::kStatusFilename);
  
  // Convert to MB
  ram /= 1024;
  std::stringstream ram_mb;
  ram_mb << std::fixed << std::setprecision(1) << ram;
  return (ram_mb).str();
 }

// @.@ DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  return LinuxParser::getParsedValueWithKey<string>(LinuxParser::kUidKey, to_string(pid) + LinuxParser::kStatusFilename);
}

// @.@ DONE: Read and return the user associated with a process
// Because of the different string format, decided to do here the reading part from the file
string LinuxParser::User(int pid) { 
  // the line we want has user:x:uid so we need 4 strings
  std::string line, user, x; 
  int uid;
  // get uid of the process, so one more string
  int uid_process = std::stoi(LinuxParser::Uid(pid)); 

  std::ifstream filestream(LinuxParser::kPasswordPath);
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> uid){
        if (uid == uid_process){
            return user;
        }
      }
    }
  }
  return user; 
}

// @.@ DONE: Read and return the uptime of a process
long LinuxParser::StartTimeProcess(int pid) { 
  std::vector<std::string> pid_status = getParsePidStatus(to_string(pid));

  // 21 starttime - Time when the process started, measured in clock ticks
  return std::stol(pid_status[21]); 
}

// #############################
// # @.@ NEW: Helper templates #
// #############################

// get values from a file using a key, mimics what was given in the examples above
template <typename T> T LinuxParser::getParsedValueWithKey(std::string const &desired_key, std::string const &filename){
  std::string line, key;
  T value;

   std::ifstream stream(kProcDirectory + filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == desired_key) {
          return value;
        }
      }
    }
  }
  return value;
}

// Parse with  key for things related to Processor::Utilization, looking to you Jiffies. 
template <typename T> T LinuxParser::getParsedValueWithKey(std::string const &desired_key){
  std::string line, key, value;
  T cpu_values;

   std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == desired_key) {
          while (linestream >> value){
            cpu_values.emplace_back(value);
          }
        }
      }
    }
  }
  return cpu_values;
}

// Parse values without using key
template <typename T> T LinuxParser::getParsedValue(std::string const &filename){
  string line;
  T value;

  std::ifstream stream(kProcDirectory + filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
}

// helper function to read proc/pid/status
std::vector<std::string> LinuxParser::getParsePidStatus(std::string const &pid){
  string line;
  std::vector<std::string> value; //hmm, well could always remove the if ...

  std::ifstream stream(kProcDirectory + pid + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    
    // split string by whitespaces -> https://stackoverflow.com/q/43106920/9004795
    std::istream_iterator<std::string> beg(linestream), end;
    std::vector<std::string> value(beg,end);
    return value; 
  }
  return value;
}
