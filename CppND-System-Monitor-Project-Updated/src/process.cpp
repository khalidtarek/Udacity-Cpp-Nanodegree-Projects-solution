#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>


#include "linux_parser.h"

using std::string;

// Initialize process ID variable pid_ with parameter pid
Process::Process(int pid) : pid_(pid) {}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  double jiffiesTime, idleTime;
  // Save active Jiffies for CPU utilization comparison
  processActiveJiffies_ = LinuxParser::ActiveJiffies(pid_);

  jiffiesTime = (processActiveJiffies_ / sysconf(_SC_CLK_TCK));
  idleTime = LinuxParser::UpTime() - jiffiesTime;

  // Return CPU utilization Jiffies time / Idle time
  return jiffiesTime / idleTime;
  ;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// Return Process CPU id
string Process::Cpu() { return LinuxParser::Cpu(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return this->processActiveJiffies_ > a.processActiveJiffies_;
}
