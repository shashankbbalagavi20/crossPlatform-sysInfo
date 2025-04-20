cmake_minimum_required(VERSION 3.10)
project(crossPlatform-sysInfo)

set(CMAKE_CXX_STANDARD 17)

set(COMMON_WINDOWS_SRC
    src/windows/sysinfo_windows.cpp
)

set(COMMON_LINUX_SRC
    src/linux/sysinfo_linux.cpp
)

set(COMMON_SRC
    src/sysinfo.cpp
)

add_library(crossPlatform-sysInfo ${COMMON_SRC})

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_sources(crossPlatform-sysInfo PRIVATE ${COMMON_WINDOWS_SRC})
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_sources(crossPlatform-sysInfo PRIVATE ${COMMON_LINUX_SRC})
else()
    message(WARNING "Unsupported platform: crossPlatform-sysInfo does not ${CMAKE_SYSTEM_NAME} operating system implemenation yet")
endif()

target_include_directories(crossPlatform-sysInfo
    PUBLIC
        include
)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.16.0
    FIND_PACKAGE_ARGS NAMES Gtest Configs "" Module
)

if(NOT googletest_SOURCE_DIR)
    FetchContent_MakeAvailable(googletest)
endif()

add_executable(test_sysinfo
    tst/test_sysinfo.cpp
)

target_include_directories(test_sysinfo
    PRIVATE
        include
        ${googletest_SOURCE_DIR}/include
)

target_link_libraries(test_sysinfo
    crossPlatform-sysInfo
    gtest_main
)

enable_testing()
add_test(NAME test_sysinfo COMMAND test_sysinfo)

add_custom_target(run_tests
    COMMAND $<TARGET_FILE:test_sysinfo>
    DEPENDS test_sysinfo
)