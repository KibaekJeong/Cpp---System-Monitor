#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"
#include "format.h"

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
    filestream.close();
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version,kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
    stream.close();
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
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  string key;
  string line;
  string value,kb;
  float total_mem =-1.0;
  float free_mem =-1.0;
  if (filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream>>key>>value>>kb){
        if(key == "MemTotal:"){
          total_mem = std::stof(value);
        }
        else if (key == "MemFree:"){
          free_mem = std::stof(value);
        }
        if ((total_mem > 0) && (free_mem > 0)){
          break;
        }
      }
    }
    filestream.close();
  }
  return static_cast <float>((total_mem - free_mem)/ total_mem);
}


float LinuxParser::cpuUtil(int pid){
  long int active_jiff = LinuxParser::ActiveJiffies(pid);

  long int seconds = LinuxParser::UpTime() - LinuxParser::UpTime(pid);
  if (seconds<=0){return 0.0;}
  
  return static_cast<float> (1.0*((active_jiff/sysconf(_SC_CLK_TCK))/seconds));
}


// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  string line;
  string uptime;
  string idle;
  if (filestream.is_open()){
    std::getline(filestream,line);
    std::istringstream linestream(line);
    linestream>>uptime>>idle;
    filestream.close();
  }
  try{
    return std::stol(uptime);
  }catch (const std::invalid_argument& arg){
    return NULL;
  }
}


// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  std::ifstream filestream(kProcDirectory+kStatFilename);
  string key;
  string line;
  string kUser, kNice, kSystem, kIdle, kIOwait, KIRQ, ksoftIRQ, kSteal, kGuest, kGuestNice;
  if (filestream.is_open()){
    std::getline(filestream,line);
    std::istringstream linestream(line);
    linestream >>key>> kUser >> kNice >> kSystem >> kIdle >> kIOwait >> KIRQ >> ksoftIRQ >> kSteal >> kGuest >> kGuestNice;
    filestream.close();
  }
  try{
    return (std::stol(kUser)+std::stol(kNice)+std::stol(kSystem)+std::stol(kIdle)+std::stol(kIOwait)+std::stol(KIRQ)+std::stol(ksoftIRQ)+std::stol(kSteal));
  }catch (const std::invalid_argument& arg){
    return NULL;
  }
}

// DONE: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatFilename);
  string shlp;
  string line;
  string kUtime, kStime, kCUtime, kCStime;
  if (filestream.is_open()){
    std::getline(filestream,line);
    std::istringstream linestream(line);
    for (int i=1; i<14; i ++){
      linestream >> shlp;
    }
    linestream>> kUtime >> kStime>> kCUtime >> kCStime;
    filestream.close();
  }
  try{
    return (std::stol(kUtime)+std::stol(kStime)+std::stol(kCUtime)+std::stol(kCStime));
  }catch (const std::invalid_argument& arg){
    return NULL;
  }
}


// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::ifstream filestream(kProcDirectory+kStatFilename);
  string key;
  string line;
  string kUser, kNice, kSystem, kIdle, kIOwait, KIRQ, ksoftIRQ, kSteal, kGuest, kGuestNice;
  if (filestream.is_open()){
    std::getline(filestream,line);
    std::istringstream linestream(line);
    linestream >> key >> kUser >> kNice >> kSystem >> kIdle >> kIOwait >> KIRQ >> ksoftIRQ >> kSteal >> kGuest >> kGuestNice;
    filestream.close();
  }
  try{
    return (std::stol(kUser)+std::stol(kNice)+std::stol(kSystem)+std::stol(KIRQ)+std::stol(ksoftIRQ)+std::stol(kSteal));
  }catch (const std::invalid_argument& arg){
    return NULL;
  }
}


// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::ifstream filestream(kProcDirectory+kStatFilename);
  string key;
  string line;
  string kUser, kNice, kSystem, kIdle, kIOwait, KIRQ, ksoftIRQ, kSteal, kGuest, kGuestNice;
  if (filestream.is_open()){
    std::getline(filestream,line);
    std::istringstream linestream(line);
    linestream >> key >> kUser >> kNice >> kSystem >> kIdle >> kIOwait >> KIRQ >> ksoftIRQ >> kSteal >> kGuest >> kGuestNice;
    filestream.close();
  }
  try{
    return (std::stol(kIdle)+ std::stol(kIOwait));
  }catch (const std::invalid_argument& arg){
    return NULL;
  }
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> CpuUtil;
  string value, line, key;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line);
    while(linestream >> key){
      if(key != "cpu"){
        CpuUtil.emplace_back(key);
      }
    }
    stream.close();
  }
  return CpuUtil;
 }

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  std::ifstream filestream(kProcDirectory+kStatFilename);
  string key;
  string line;
  string processes_;
  if (filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key >> processes_;
      if (key =="processes"){
        filestream.close();
        return std::stoi(processes_);
      }
    }
    filestream.close();
  }
  return NULL;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::ifstream filestream(kProcDirectory+kStatFilename);
  string key;
  string line;
  string rprocesses_;
  if (filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key >> rprocesses_;
      if (key =="procs_running"){
        filestream.close();
        return std::stoi(rprocesses_);
      }
    }
    filestream.close();
  }
  return NULL;
}
 

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  std::ifstream filestream(kProcDirectory+to_string(pid)+kCmdlineFilename);
  string line;
  if (filestream.is_open()){
    std::getline(filestream,line);
    filestream.close();
    return line;

  }
  return nullptr;
}
 

// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatusFilename);
  string key;
  string line;
  string mem;
  if (filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key >> mem;
      //Using physical memory size instead of virtual memory size. 
      if (key =="VmData:"){

        const long memory = std::stol(mem)/1024.0;
        filestream.close();
        return std::to_string(memory);
      }
    }
    filestream.close();
  }
  return nullptr;
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatusFilename);
  string key;
  string line;
  string uid;
  if (filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key >> uid;
      if (key =="Uid:"){
        filestream.close();
        return uid;
      }
    }
    filestream.close();
  }
  return nullptr;
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  std::ifstream filestream(kPasswordPath);
  string line;
  unsigned int id_find = std::stoi(Uid(pid));
  std::string username,x;
  unsigned int user_id;
  if (filestream.is_open()){
    while(std::getline(filestream,line)){
      std::replace(line.begin(),line.end(),':',' ');
      std::istringstream lineStream(line);
      lineStream >> username >> x >> user_id;
      if (user_id == id_find){
        filestream.close();
        return username;
      }
    }
    filestream.close();
  }
  return nullptr;
}

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
   std::ifstream filestream(kProcDirectory+to_string(pid)+kStatFilename);
  string key;
  string line;
  string value;
  long uptime =0;
  if (filestream.is_open()){
    while( std::getline(filestream,line)){
      std::istringstream linestream(line);
      for (int i =0; i< 22;++i){
        linestream >> value;
        try{
            uptime = std::stol(value)/sysconf(_SC_CLK_TCK);
            filestream.close();
            return LinuxParser::UpTime() - uptime;
        }catch (const std::invalid_argument& arg){
          return 0.;
        }
      }
    }
    filestream.close();
  }
  return NULL;
}

