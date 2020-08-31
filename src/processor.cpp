#include "processor.h"
#include "linux_parser.h"
#include <iostream>

// TODO: Return the aggregate CPU utilization
Processor::Processor(){
    prev_all = LinuxParser::Jiffies();
    prev_idle = LinuxParser::IdleJiffies();

}

double Processor::Utilization() { 
    

    current_all = LinuxParser::Jiffies();
    current_idle = LinuxParser::IdleJiffies();

    float diff_idle = current_idle - prev_idle;
    float diff_all = current_all - prev_all;

    double result = static_cast<double> (diff_all - diff_idle)/diff_all;

    prev_all = current_all;
    prev_idle = current_idle;

    return result;
}