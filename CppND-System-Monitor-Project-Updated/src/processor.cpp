#include "processor.h"
#include "linux_parser.h"

using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization(string cpuNumber) { 
    // Agregated utilication for CPU ID is the Active Jiffies / Total Jiffies for this CPU ID
    return (float)LinuxParser::ActiveJiffies(cpuNumber) / LinuxParser::Jiffies(cpuNumber) ;
}
