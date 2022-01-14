#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  // Simple Constructor -> maybe not
  Process(int pid, long hertz) : pid_(pid), hertz_(hertz) { cpu_utilization_ = CpuUtilization(); }
  int Pid();                               // @.@ DONE: See src/process.cpp
  std::string User();                      // @.@ DONE: See src/process.cpp
  std::string Command();                   // @.@ DONE: See src/process.cpp
  float CpuUtilization();                  // @.@ DONE: See src/process.cpp
  std::string Ram();                       // @.@ DONE: See src/process.cpp
  long int UpTimeProcess();                // @.@ DONE: See src/process.cpp
  bool operator<(Process const& a) const;  // @.@ DONE: See src/process.cpp

  // @.@ DONE: Declare any necessary private members
 private:
    int pid_{0};
    long hertz_{0};
    float cpu_utilization_{0.0};

};

#endif