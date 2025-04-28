#ifdef _WIN32 // This file has to be only compiled in windows

#include "sysinfo_windows.hpp"
#include "windows.h"
#include <string>
#include <sstream>
#include <memory>
#include <stdexcept>

namespace sysinfo {

std::string SysInfoForWindows::getOperatingSystem(){
    return getWindowsVersion();
}

std::string SysInfoForWindows::getCPUModel(){
    return getProcessorName();
}

int SysInfoForWindows::getNumberOfCores(){
    return getNumberOfProcessor();
}

/*Documentation link: https://learn.microsoft.com/en-us/windows/win32/api/ */
std::string getWindowsVersion() {
    OSVERSIONINFOEXW osvinfo;
    ZeroMemory(&osvinfo, sizeof(OSVERSIONINFOEXW));
    osvinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);

    // Use RtlGetVersion instead of GetVersionExW
    typedef LONG(WINAPI* RtlGetVersionPtr)(OSVERSIONINFOEXW*);
    HMODULE hModule = GetModuleHandleW(L"ntdll.dll");
    if (!hModule) {
        throw std::runtime_error("Failed to load ntdll.dll");
    }

    RtlGetVersionPtr rtlGetVersion = (RtlGetVersionPtr)GetProcAddress(hModule, "RtlGetVersion");
    if (!rtlGetVersion) {
        throw std::runtime_error("Failed to locate RtlGetVersion");
    }

    if (rtlGetVersion(&osvinfo) != 0) {
        throw std::runtime_error("RtlGetVersion failed");
    }

    std::wstringstream ss;
    ss << L"Windows ";
    if (osvinfo.dwMajorVersion == 10) {
        ss << L"10";
    } else if (osvinfo.dwMajorVersion == 6) {
        if (osvinfo.dwMinorVersion == 3) ss << L"8.1";
        if (osvinfo.dwMinorVersion == 2) ss << L"8";
        if (osvinfo.dwMinorVersion == 1) ss << L"7";
        if (osvinfo.dwMinorVersion == 0) ss << L"Vista";
    } else if (osvinfo.dwMajorVersion == 5) {
        if (osvinfo.dwMinorVersion == 1) ss << L"XP";
        if (osvinfo.dwMinorVersion == 0) ss << L"2000";
    } else {
        ss << osvinfo.dwMajorVersion << L"." << osvinfo.dwMinorVersion;
    }

    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, ss.str().c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (bufferSize == 0) {
        DWORD error = GetLastError();
        throw std::runtime_error("WideCharToMultiByte (size) failed");
    }
    std::string utf8String(bufferSize, 0);
    WideCharToMultiByte(CP_UTF8, 0, ss.str().c_str(), -1, utf8String.data(), bufferSize, nullptr, nullptr);
    utf8String.resize(bufferSize - 1); // Remove the null terminator
    return utf8String;
}

std::string getProcessorName(){
    HKEY hardwareKey;
    wchar_t processorName[256];
    DWORD bufferSize = sizeof(processorName);
    std::string result = "Unknown Processor";

    // RegOpenKeyExW() opens a specified registry key
    // RegQueryValueExW() retrieves the data associated with a specified value name within an open registry key
    // RegCloseKey() to free up system resources
    // HKEY_LOCAL_MACHINE : This is a predefined HKEY that represents the root of the local machine's configuration information
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                     L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
                     0,
                     KEY_READ,
                     &hardwareKey) != ERROR_SUCCESS) {
        DWORD error = GetLastError();
        throw std::runtime_error("RegOpenKeyExW failed with error");
    }

    // Use a RAII wrapper to ensure the key is closed.
    std::unique_ptr<std::remove_pointer_t<HKEY>, decltype(&RegCloseKey)> key_guard(hardwareKey, RegCloseKey);

    if (RegQueryValueExW(hardwareKey,
                        L"ProcessorNameString",
                        NULL,
                        NULL,
                        (LPBYTE)processorName,
                        &bufferSize) == ERROR_SUCCESS) {
        int utf8BufferSize = WideCharToMultiByte(CP_UTF8, 0, processorName, -1, nullptr, 0, nullptr, nullptr);
        if (utf8BufferSize == 0)
        {
                DWORD error = GetLastError();
                throw std::runtime_error("WideCharToMultiByte (size) in getProcessorNameInternal failed");
        }
        std::string utf8ProcessorName(utf8BufferSize, 0);
        WideCharToMultiByte(CP_UTF8, 0, processorName, -1, utf8ProcessorName.data(), utf8BufferSize, nullptr, nullptr);
        utf8ProcessorName.resize(utf8BufferSize - 1);
        return utf8ProcessorName;
    }
    else{
        DWORD error = GetLastError();
        throw std::runtime_error("RegQueryValueExW failed with error");
    }
    return result;
}

std::string getNumberOfProcessor(){
    // SYSTEM_INFO: This is a Windows API structure
    // GetSystemInfo() will then populate the members of this structure with the relevant system details
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    return std::to_string(sysInfo.dwNumberOfProcessors);
}
} // namespace sysinfo

#endif // _WIN32