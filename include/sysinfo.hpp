#ifndef CROSSPLATFORM_SYSINFO_HPP
#define CROSSPLATFORM_SYSINFO_HPP

#include <string>
#include <memory>

namespace sysinfo {

class SystemInfo {
    public:
        virtual ~SystemInfo() = default;

        virtual std::string getOperatingSystem() = 0;
        virtual std::string getCPUModel() = 0;
        virtual int getNumberOfCores() = 0;

};

// Factory function to create required SystemInfo object
std::unique_ptr<SystemInfo> createSysInfo();

} // namespace sysinfo

#endif CROSSPLATFORM_SYSINFO_HPP