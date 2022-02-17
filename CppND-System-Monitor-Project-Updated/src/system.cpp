

#include <unistd.h>

#include <cstddef>

#include <string>
#include <vector>


#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    processes_.clear();
    vector<int> pids = LinuxParser::Pids();
    // Loop Over all PIDs vector elements to create process instance 
    // And append it to process_ vector
    for (int pid : pids){
        Process process(pid);
        processes_.emplace_back(process);
    }
    // Sort processes in Decencding order 
    sort(processes_.begin(), processes_.end(), [](Process const& a, Process const& b) {return a < b;});
    return processes_;
}

// TODO: Return the system's kernel identifier (string)
string System::Kernel() const { return LinuxParser::Kernel(); }

// Return the number of CPUs in the system
int System::NumberOfCpus() const { return LinuxParser::NumberOfCpus(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
string System::OperatingSystem() const { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
