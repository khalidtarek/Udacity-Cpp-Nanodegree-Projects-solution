#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <filesystem>
#include <string>
#include <vector>

using std::all_of;
using std::getline;
using std::ifstream;
using std::istringstream;
using std::replace;
using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ' ', '_');
      replace(line.begin(), line.end(), '=', ' ');
      replace(line.begin(), line.end(), '"', ' ');
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// Return number of CPUs in the system
int LinuxParser::NumberOfCpus() {
  string line, key;
  int cpus = 0;
  ifstream filestream(kProcDirectory + kCpuinfoFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      while (linestream >> key) {
        if (key == "processor") {
          cpus++;
        }
      }
    }
  }
  return cpus;
}

// BONUS: Update this to use filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, ignore;
  float memTotal, memFree;
  ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    // Get total memory
    getline(stream, line);
    istringstream linestream(line);
    linestream >> ignore >> memTotal;

    // Get free memory
    getline(stream, line);
    istringstream linestream2(line);
    linestream2 >> ignore >> memFree;
  }
  return (memTotal - memFree) / memTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line, upTime;
  ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    // Get up time
    getline(stream, line);
    istringstream linestream(line);
    linestream >> upTime;
  }
  return stol(upTime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies(string cpuNumber) {
  return ActiveJiffies(cpuNumber) + IdleJiffies(cpuNumber);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line, ignore;
  ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  long utime, stime, cutime, cstime;
  if (filestream.is_open()) {
    getline(filestream, line);
    istringstream linestream(line);
    // Ignore first 13 values
    for (int i = 0; i < 13; i++) linestream >> ignore;
    // Get active jiffies
    linestream >> utime >> stime >> cutime >> cstime;
    return utime + stime + cutime + cstime;
  }
  return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies(string cpuNumber) {
  vector<string> values = CpuUtilization(cpuNumber);
  return stol(values[CPUStates::kUser_]) + stol(values[CPUStates::kNice_]) +
         stol(values[CPUStates::kSystem_]) + stol(values[CPUStates::kIRQ_]) +
         stol(values[CPUStates::kSoftIRQ_]) + stol(values[CPUStates::kSteal_]) +
         stol(values[CPUStates::kGuest_]) +
         stol(values[CPUStates::kGuestNice_]);
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies(string cpuNumber) {
  vector<string> values = CpuUtilization(cpuNumber);
  return stol(values[CPUStates::kIdle_]) + stol(values[CPUStates::kIOwait_]);
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization(string cpuNumber) {
  vector<string> cpu;
  string line, key, value;
  ifstream filestream(kProcDirectory + kStatFilename);
  // Get CPU utilization for CPU ID
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      while (linestream >> key) {
        if (key == cpuNumber) {
          while (linestream >> key) {
            cpu.emplace_back(key);
          }
          return cpu;
        }
      }
    }
  }
  return cpu;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key, value;
  ifstream filestream(kProcDirectory + kStatFilename);
  // Get the total number of processes
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, value;
  ifstream filestream(kProcDirectory + kStatFilename);
  // Get the number of running processes
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  // Get the command associated with a process
  if (filestream.is_open()) {
    getline(filestream, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line, key, value;
  ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  // Get the memory used by a process
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          return to_string(stoi(value) / 1024);
        }
      }
    }
  }
  return "0";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line, key, value;
  ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  // Get the user ID associated with a process
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line, key, value, user, ignore;
  ifstream filestream(kPasswordPath);
  // Get the user associated with a process
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ':', ' ');
      istringstream linestream(line);
      while (linestream >> value >> ignore >> key) {
        if (key == Uid(pid)) {
          user = value;
        }
      }
    }
  }
  return user;
}

// Return Process CPU id
string LinuxParser::Cpu(int pid) {
  string line, key, value, cpu, ignore;
  ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  // Get the Process CPU id
  if (filestream.is_open()) {
    getline(filestream, line);
    istringstream linestream(line);
    for (int i = 0; i < 38; i++) linestream >> ignore;
    linestream >> value;
    cpu = "cpu" + to_string(stoi(value) + 1);
  }
  return cpu;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string value, line;
  ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  // Get the uptime of a process
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      for (int i = 0; i < 22; i++) {
        linestream >> value;
      }
      return (stol(value) / sysconf(_SC_CLK_TCK));
    }
  }
  return 0;
}
