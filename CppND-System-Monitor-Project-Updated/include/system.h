#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

using std::string;
using std::vector;

class System {
 public:
  Processor& Cpu();                // TODO: See src/system.cpp
  vector<Process>& Processes();    // TODO: See src/system.cpp
  float MemoryUtilization();       // TODO: See src/system.cpp
  long UpTime();                   // TODO: See src/system.cpp
  int TotalProcesses();            // TODO: See src/system.cpp
  int RunningProcesses();          // TODO: See src/system.cpp
  string Kernel() const;           // TODO: See src/system.cpp
  string OperatingSystem() const;  // TODO: See src/system.cpp
  int NumberOfCpus() const;        // Return number of CPUs in the system

  // TODO: Define any necessary private members
 private:
  Processor cpu_ = {};
  vector<Process> processes_ = {};
};

#endif
