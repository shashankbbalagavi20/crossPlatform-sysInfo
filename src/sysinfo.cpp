#include "sysinfo.hpp"
#include <memory>

// file inclusion based on operating system
#ifdef _WIN32
#include "windows/sysinfo_windows.hpp"
#elif defined(__linux__)
#include "linux/sysinfo_linux.hpp"
#else
#include <stdexcept>
#endif

namespace sysinfo{

std::unique_ptr<SystemInfo> createSysInfo(){
    #ifdef _WIN32
        return std::make_unique<SysInfoForWindows>();
    #elif defined(__linux__)
        return std::make_unique<LinuxSystemInfo>();
    #else
        throw std::runtime_error("Unsupported Operating System");
    #endif
}
} // namespace sysinfo
