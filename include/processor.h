#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor();
  double Utilization();  // DONE: See src/processor.cpp

  // DONE: Declare any necessary private members
 private:
 long int current_all;
 long int current_idle;
 long int prev_all;
 long int prev_idle;
};

#endif