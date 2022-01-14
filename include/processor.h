#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // @.@ Done: See src/processor.cpp

  // @.@ Done: Declare any necessary private members
 private:
    long prev_total_time_{0};
    long prev_idle_time_{0};
    long total_time_{0};
    long idle_time_{0}; 
};

#endif