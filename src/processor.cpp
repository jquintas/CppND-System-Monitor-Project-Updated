#include <thread>
#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    // *.* save Jiffies time
    
    prev_total_time_ = LinuxParser::Jiffies();
    prev_idle_time_ = LinuxParser::IdleJiffies();

    // *.* wait one second according to this https://stackoverflow.com/a/12604028/9004795
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    //*.* discount actual prev_time to actual_time
    total_time_ = LinuxParser::Jiffies() - prev_total_time_;
    idle_time_ = LinuxParser::IdleJiffies() - prev_idle_time_;

    return (float)(total_time_ - idle_time_) / float(total_time_); 
    
   return 0;
}