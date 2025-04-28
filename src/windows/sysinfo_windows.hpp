#ifndef SYSINFO_WINDOWS_HPP
#define SYSINFO_WINDOWS_HPP

#include "sysinfo.hpp"
#include <string>

namespace sysinfo {

class SysInfoForWindows : public SystemInfo
{
public:
    std::string getCPUModel() override;
    std::string getOperatingSystem() override;
    int getNumberOfCores() override;
private:
    std::string getWindowsVersion();
    std::string getProcessorName();
    int getNumberOfProcessor();
};

} // namespace sysinfo

#endif // SYSINFO_WINDOWS_HPP