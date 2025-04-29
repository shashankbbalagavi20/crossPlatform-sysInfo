#ifndef SYSINFO_LINUX_HPP
#define SYSINFO_LINUX_HPP

#include "sysinfo.hpp"
#include <string>

namespace sysinfo {

class SysInfoForLinux : public SystemInfo {
    public :
     std::string getOperatingSystem() override;
     std::string getCPUModel() override;
     int getNumberOfCores() override;
};

} // namespace sysinfo

#endif // SYSINFO_LINUX_HPP 