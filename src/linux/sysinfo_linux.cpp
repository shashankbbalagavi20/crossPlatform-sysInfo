#ifdef __linux__

#include "sysinfo_linux.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <cstring>

namespace sysinfo {
    
std::string SysInfoForLinux::getOperatingSystem(){
    return "Linux";
}

std::string SysInfoForLinux::getCPUModel(){
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    while(std::getline(cpuinfo, line)){
        std::stringstream ss(line);
        std::string key;
        std::string value;
        ss >> key;
        if(key == "model" || key == "model\ name"){
            std::getline(ss >> std::ws, value);
            return value;
        }
    }
    return "Unknown Processor";
}

int SysInfoForLinux::getNumberOfCores(){
    long nprocs = sysconf(_SC_NPROCESSORS_ONLN);
    if(nprocs < 0){
        std::cerr << "Failed to get number of processors: " << strerror(errno) << std::endl;
        return 1;
    }
    return static_cast<int>(nprocs);
}

} // namespace sysinfo

#endif // __linux__