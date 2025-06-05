#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <iomanip>
#include <functional>
#include "pfc-mini.hpp"

#ifdef _WIN32
#include <windows.h>
#include <intrin.h>
#pragma comment(lib, "kernel32.lib")
#endif

// Basic operation test variables
volatile int add_var1 = 1, add_var2 = 2;
volatile int assign_var1 = 1, assign_var2 = 2;
volatile int compare_var1 = 1, compare_var2 = 2;
volatile double divide_var1 = 100.0, divide_var2 = 3.0;
volatile int multiply_var1 = 3, multiply_var2 = 4;
int test_array[1000000];
volatile int index_result;

void get_cpu_info() {
    std::cout << "\n=== CPU Information ===" << std::endl;
    
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
    
    // Get frequency info
    LARGE_INTEGER frequency;
    if (QueryPerformanceFrequency(&frequency)) {
        std::cout << "Performance Counter Frequency: " << frequency.QuadPart << " Hz" << std::endl;
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
    
    std::cout << "Timer resolution: " << pfc::in_s(pfc::get_timer_resolution()) * 1e9 << " ns" << std::endl;
    
    get_cpu_info();
}

// Operation measurement functions
void measure_basic_operations() {
    std::cout << "\n=== Basic Operations Timing Measurement ===" << std::endl;
    
    const int iterations = 10000000;
    std::ofstream csv_file("basic_operations_timing.csv");
    csv_file << "Operation,Time_ns,Iterations" << std::endl;
    
    // Initialize test array for indexing
    for (int i = 0; i < 1000000; ++i) {
        test_array[i] = i;
    }
    
    // Add operation
    auto add_time = pfc::timed_run(iterations, []() {
        volatile int result = add_var1 + add_var2;
        (void)result; // Prevent optimization
    });
    double add_ns = pfc::in_s(add_time) * 1e9;
    std::cout << "Add: " << std::fixed << std::setprecision(3) << add_ns << " ns" << std::endl;
    csv_file << "Add," << add_ns << "," << iterations << std::endl;
    
    // Assign operation
    auto assign_time = pfc::timed_run(iterations, []() {
        assign_var1 = assign_var2;
    });
    double assign_ns = pfc::in_s(assign_time) * 1e9;
    std::cout << "Assign: " << std::fixed << std::setprecision(3) << assign_ns << " ns" << std::endl;
    csv_file << "Assign," << assign_ns << "," << iterations << std::endl;
    
    // Compare operation
    auto compare_time = pfc::timed_run(iterations, []() {
        volatile bool result = compare_var1 < compare_var2;
        (void)result; // Prevent optimization
    });
    double compare_ns = pfc::in_s(compare_time) * 1e9;
    std::cout << "Compare: " << std::fixed << std::setprecision(3) << compare_ns << " ns" << std::endl;
    csv_file << "Compare," << compare_ns << "," << iterations << std::endl;
    
    // Divide operation
    auto divide_time = pfc::timed_run(iterations, []() {
        volatile double result = divide_var1 / divide_var2;
        (void)result; // Prevent optimization
    });
    double divide_ns = pfc::in_s(divide_time) * 1e9;
    std::cout << "Divide: " << std::fixed << std::setprecision(3) << divide_ns << " ns" << std::endl;
    csv_file << "Divide," << divide_ns << "," << iterations << std::endl;
    
    // Multiply operation
    auto multiply_time = pfc::timed_run(iterations, []() {
        volatile int result = multiply_var1 * multiply_var2;
        (void)result; // Prevent optimization
    });
    double multiply_ns = pfc::in_s(multiply_time) * 1e9;
    std::cout << "Multiply: " << std::fixed << std::setprecision(3) << multiply_ns << " ns" << std::endl;
    csv_file << "Multiply," << multiply_ns << "," << iterations << std::endl;
    
    // Index operation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 999999);
    
    auto index_time = pfc::timed_run(iterations, [&]() {
        int idx = dis(gen) % 1000000;
        index_result = test_array[idx];
    });
    double index_ns = pfc::in_s(index_time) * 1e9;
    std::cout << "Index: " << std::fixed << std::setprecision(3) << index_ns << " ns" << std::endl;
    csv_file << "Index," << index_ns << "," << iterations << std::endl;
    
    csv_file.close();
    std::cout << "Basic operations results saved to 'basic_operations_timing.csv'" << std::endl;
}

void test_basic_operations() {
    std::cout << "\n=== Basic Operations Correctness Tests ===" << std::endl;
    
    // Test add
    volatile int test_add_result = add_var1 + add_var2;
    std::cout << "Add test: " << add_var1 << " + " << add_var2 << " = " << test_add_result;
    std::cout << (test_add_result == 3 ? " - PASSED" : " - FAILED") << std::endl;
    
    // Test assign
    volatile int old_val = assign_var1;
    assign_var1 = assign_var2;
    std::cout << "Assign test: " << old_val << " := " << assign_var2 << ", result = " << assign_var1;
    std::cout << (assign_var1 == assign_var2 ? " - PASSED" : " - FAILED") << std::endl;
    assign_var1 = old_val; // Reset
    
    // Test compare
    volatile bool test_compare_result = compare_var1 < compare_var2;
    std::cout << "Compare test: " << compare_var1 << " < " << compare_var2 << " = " << (test_compare_result ? "true" : "false");
    std::cout << (test_compare_result ? " - PASSED" : " - FAILED") << std::endl;
    
    // Test divide
    volatile double test_divide_result = divide_var1 / divide_var2;
    std::cout << "Divide test: " << divide_var1 << " / " << divide_var2 << " = " << std::fixed << std::setprecision(3) << test_divide_result;
    std::cout << (test_divide_result > 33.0 && test_divide_result < 34.0 ? " - PASSED" : " - FAILED") << std::endl;
    
    // Test multiply
    volatile int test_multiply_result = multiply_var1 * multiply_var2;
    std::cout << "Multiply test: " << multiply_var1 << " * " << multiply_var2 << " = " << test_multiply_result;
    std::cout << (test_multiply_result == 12 ? " - PASSED" : " - FAILED") << std::endl;
    
    // Test index
    test_array[500000] = 42;
    volatile int test_index_result = test_array[500000];
    std::cout << "Index test: array[500000] = " << test_index_result;
    std::cout << (test_index_result == 42 ? " - PASSED" : " - FAILED") << std::endl;
}

int main() {
    try {
        print_system_info();
        test_basic_operations();
        measure_basic_operations();
        
        std::cout << "\n=== Example 1 completed successfully ===" << std::endl;
        std::cout << "CSV file generated for Excel analysis:" << std::endl;
        std::cout << "- basic_operations_timing.csv" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}