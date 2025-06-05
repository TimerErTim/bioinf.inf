#include <iostream>
#include <iomanip>
#include "pfc-mini.hpp"

// For getting CPU info
#ifdef _WIN32
#include <windows.h>
#include <intrin.h>
#pragma comment(lib, "kernel32.lib")
#endif

void print_cpu_info() {    
#ifdef _WIN32
    // Get CPU name
    int cpu_info[4];
    char cpu_brand[49];
    memset(cpu_brand, 0, sizeof(cpu_brand));
    
    __cpuid(cpu_info, 0x80000000);
    if ((unsigned int)cpu_info[0] >= 0x80000004) {
        __cpuid((int*)(cpu_brand), 0x80000002);
        __cpuid((int*)(cpu_brand + 16), 0x80000003);
        __cpuid((int*)(cpu_brand + 32), 0x80000004);
        std::cout << "CPU: " << cpu_brand << std::endl;
    }
    
    // Get system info
    SYSTEM_INFO sys_info;
    GetSystemInfo(&sys_info);
    
    std::cout << "Architecture: ";
    switch (sys_info.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64:
            std::cout << "x64 (AMD64)";
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            std::cout << "x86 (Intel)";
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            std::cout << "ARM";
            break;
        case PROCESSOR_ARCHITECTURE_ARM64:
            std::cout << "ARM64";
            break;
        default:
            std::cout << "Unknown (" << sys_info.wProcessorArchitecture << ")";
    }
    std::cout << std::endl;
    
    std::cout << "Logical Processors: " << sys_info.dwNumberOfProcessors << std::endl;
    std::cout << "Page Size: " << sys_info.dwPageSize << " bytes" << std::endl;
    
    // Get CPU frequency using HKEY_LOCAL_MACHINE
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD mhz;
        DWORD size = sizeof(DWORD);
        if (RegQueryValueExA(hKey, "~MHz", NULL, NULL, (LPBYTE)&mhz, &size) == ERROR_SUCCESS) {
            std::cout << "CPU Base Frequency: " << mhz << " MHz" << std::endl;
        }
        RegCloseKey(hKey);
    }    
#else
    std::cout << "CPU: Information not available on this platform" << std::endl;
    std::cout << "Architecture: Non-Windows platform" << std::endl;
#endif
}

void print_system_info() {
    std::cout << "=== System Information ===" << std::endl;
    std::cout << "Compiler: ";
#ifdef _MSC_VER
    std::cout << "Microsoft Visual C++ " << _MSC_VER;
#elif defined(__GNUC__)
    std::cout << "GCC " << __GNUC__ << "." << __GNUC_MINOR__;
#else
    std::cout << "Unknown";
#endif
    std::cout << std::endl;
        
    print_cpu_info();

	std::cout << "Timer resolution: " << pfc::in_s(pfc::get_timer_resolution()) * 1e9 << " ns" << std::endl;
}