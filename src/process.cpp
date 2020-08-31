#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


Process::Process(int pid):pId_(pid), cpuUsage_(CpuUtilization()) {
}
// DONE: Return this process's ID
int Process::Pid() { return pId_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {
    total_time_ = LinuxParser::ActiveJiffies(pId_)/sysconf(_SC_CLK_TCK);
    seconds_ = LinuxParser::UpTime(pId_);
    if (seconds_<=0)return 0.0;
    return total_time_/seconds_;
}

// DONE: Return the command that generated this process
string Process::Command() { 
    string command = LinuxParser::Command(pId_);
    if (command.length()>50){
        command.resize(50);
        return command.append("...");
    }
    return command;
}
// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pId_); }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pId_); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pId_); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return (cpuUsage_ < a.cpuUsage_)? true:false; 
}
bool Process::operator>(Process const& a) const { 
    return (cpuUsage_ > a.cpuUsage_)? true:false; 
}