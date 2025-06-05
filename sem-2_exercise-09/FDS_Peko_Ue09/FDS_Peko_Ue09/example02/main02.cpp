#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <iomanip>
#include <functional>
#include <cmath>
#include "pfc-mini.hpp"
#include "../example01/system_info.cpp"
#include "../example01/timing_operations.cpp"

// Binary search implementations from lecture slides
int binary_search_v1(int* arr, int n, int target) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

int binary_search_v2(int* arr, int n, int target) {
    int left = 0, right = n;
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] < target) left = mid + 1;
        else right = mid;
    }
    return (left < n && arr[left] == target) ? left : -1;
}

int binary_search_v3(int* arr, int n, int target) {
    if (n == 0) return -1;
    int mid = n / 2;
    if (arr[mid] == target) return mid;
    if (arr[mid] < target) {
        int result = binary_search_v3(arr + mid + 1, n - mid - 1, target);
        return (result != -1) ? mid + 1 + result : -1;
    } else {
        return binary_search_v3(arr, mid, target);
    }
}

// Calculate theoretical runtime for binary search using actual operation timings
double calculate_theoretical_runtime(int n, bool found, const BasicOperationTimings& timings) {
    // Based on binary search complexity analysis
    const int compare_ops = static_cast<int>(std::log2(n)) + 1;
    const int arithmetic_ops = compare_ops * 2; // mid calculation, index updates  
    const int assign_ops = compare_ops * 3; // left, right, mid assignments
    const int index_ops = compare_ops; // array access operations
    
    // Calculate theoretical runtime using actual measured operation timings
    double theoretical_time = 
        compare_ops * timings.compare_ns +
        arithmetic_ops * timings.add_ns + // approximating arithmetic as addition
        assign_ops * timings.assign_ns +
        index_ops * timings.index_ns;
    
    return theoretical_time;
}

// Calculate theoretical operation counts for analysis
struct OperationCounts {
    int compare_ops;
    int arithmetic_ops;
    int assign_ops;
    int index_ops;
    int total_ops;
};

OperationCounts calculate_theoretical_operations(int n, bool found) {
    OperationCounts counts;
    counts.compare_ops = static_cast<int>(std::log2(n)) + 1;
    counts.arithmetic_ops = counts.compare_ops * 2;
    counts.assign_ops = counts.compare_ops * 3;
    counts.index_ops = counts.compare_ops;
    counts.total_ops = counts.compare_ops + counts.arithmetic_ops + counts.assign_ops + counts.index_ops;
    
    return counts;
}

void measure_binary_search() {
    std::cout << "\n=== Binary Search Analysis ===" << std::endl;
    
    // Initialize test array and basic operation variables
    for (int i = 0; i < 1000000; ++i) {
        test_array[i] = i;
    }
    
    // Measure basic operations first for theoretical calculations
    BasicOperationTimings timings = measure_all_basic_operations(10'000'000); // Faster for binary search analysis
    
    std::vector<int> sizes = {1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000};
    const int test_iterations = 1000;
    
    std::ofstream csv_file("../../../out/binary_search_analysis.csv");
    csv_file << "Size,Algorithm,Scenario,Theoretical_ns,Measured_ns,Total_Operations,Compare_Ops,Arithmetic_Ops,Assign_Ops,Index_Ops" << std::endl;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    for (int size : sizes) {
        std::cout << "\nTesting with array size: " << size << std::endl;
        
        // Create sorted array
        std::vector<int> arr(size);
        for (int i = 0; i < size; ++i) {
            arr[i] = i * 2; // Even numbers to leave gaps for "not found" tests
        }
        
        // Generate test values
        std::uniform_int_distribution<> found_dis(0, size - 1);
        std::uniform_int_distribution<> not_found_dis(1, size * 2 - 1);
        
        // Test each algorithm
        std::vector<std::pair<std::string, std::function<int(int*, int, int)>>> algorithms = {
            {"V1", binary_search_v1},
            {"V2", binary_search_v2},
            {"V3", binary_search_v3}
        };
        
        for (auto& algo : algorithms) {
            // Test "found" scenario
            auto found_time = pfc::timed_run(test_iterations, [&]() {
                int target_idx = found_dis(gen);
                int target = arr[target_idx];
                volatile int result = algo.second(arr.data(), size, target);
                (void)result;
            });
            double found_ns = pfc::in_s(found_time) * 1e9;
            
            // Test "not found" scenario
            auto not_found_time = pfc::timed_run(test_iterations, [&]() {
                int target = not_found_dis(gen);
                if (target % 2 == 0) target++; // Ensure odd number (not in array)
                volatile int result = algo.second(arr.data(), size, target);
                (void)result;
            });
            double not_found_ns = pfc::in_s(not_found_time) * 1e9;
            
            // Calculate theoretical values using actual operation timings
            OperationCounts ops_found = calculate_theoretical_operations(size, true);
            OperationCounts ops_not_found = calculate_theoretical_operations(size, false);
            
            double theoretical_found_ns = calculate_theoretical_runtime(size, true, timings);
            double theoretical_not_found_ns = calculate_theoretical_runtime(size, false, timings);
            
            std::cout << algo.first << " found: " << std::fixed << std::setprecision(3) 
                      << found_ns << " ns (theoretical: " << theoretical_found_ns << " ns, ops: " << ops_found.total_ops << ")" << std::endl;
            std::cout << algo.first << " not found: " << std::fixed << std::setprecision(3) 
                      << not_found_ns << " ns (theoretical: " << theoretical_not_found_ns << " ns, ops: " << ops_not_found.total_ops << ")" << std::endl;
            
            csv_file << size << "," << algo.first << ",Found," << theoretical_found_ns << "," << found_ns << "," 
                     << ops_found.total_ops << "," << ops_found.compare_ops << "," << ops_found.arithmetic_ops << "," 
                     << ops_found.assign_ops << "," << ops_found.index_ops << std::endl;
            csv_file << size << "," << algo.first << ",NotFound," << theoretical_not_found_ns << "," << not_found_ns << "," 
                     << ops_not_found.total_ops << "," << ops_not_found.compare_ops << "," << ops_not_found.arithmetic_ops << "," 
                     << ops_not_found.assign_ops << "," << ops_not_found.index_ops << std::endl;
        }
    }
    
    csv_file.close();
    std::cout << "\nBinary search analysis results saved to 'binary_search_analysis.csv'" << std::endl;
}

void test_correctness() {
    std::cout << "\n=== Binary Search Correctness Tests ===" << std::endl;
    
    std::vector<int> test_arr = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int size = test_arr.size();
    
    // Test found cases
    std::cout << "Testing found cases:" << std::endl;
    bool all_found_passed = true;
    for (int i = 0; i < size; ++i) {
        int target = test_arr[i];
        int result1 = binary_search_v1(test_arr.data(), size, target);
        int result2 = binary_search_v2(test_arr.data(), size, target);
        int result3 = binary_search_v3(test_arr.data(), size, target);
        
        bool passed = (result1 >= 0 && result2 >= 0 && result3 >= 0);
        std::cout << "Target " << target << ": V1=" << result1 << ", V2=" << result2 << ", V3=" << result3;
        std::cout << (passed ? " - PASSED" : " - FAILED") << std::endl;
        
        if (!passed) all_found_passed = false;
    }
    
    // Test not found cases
    std::cout << "\nTesting not found cases:" << std::endl;
    std::vector<int> not_found = {0, 2, 4, 6, 8, 20, 25};
    bool all_not_found_passed = true;
    for (int target : not_found) {
        int result1 = binary_search_v1(test_arr.data(), size, target);
        int result2 = binary_search_v2(test_arr.data(), size, target);
        int result3 = binary_search_v3(test_arr.data(), size, target);
        
        bool passed = (result1 == -1 && result2 == -1 && result3 == -1);
        std::cout << "Target " << target << ": V1=" << result1 << ", V2=" << result2 << ", V3=" << result3;
        std::cout << (passed ? " - PASSED" : " - FAILED") << std::endl;
        
        if (!passed) all_not_found_passed = false;
    }
    
    std::cout << "\nOverall correctness: " << (all_found_passed && all_not_found_passed ? "PASSED" : "FAILED") << std::endl;
}

void test_edge_cases() {
    std::cout << "\n=== Edge Case Tests ===" << std::endl;
    
    // Test empty array
    std::vector<int> empty_arr;
    int result1 = binary_search_v1(empty_arr.data(), 0, 5);
    int result2 = binary_search_v2(empty_arr.data(), 0, 5);
    int result3 = binary_search_v3(empty_arr.data(), 0, 5);
    std::cout << "Empty array search: V1=" << result1 << ", V2=" << result2 << ", V3=" << result3;
    std::cout << (result1 == -1 && result2 == -1 && result3 == -1 ? " - PASSED" : " - FAILED") << std::endl;
    
    // Test single element array
    std::vector<int> single = {42};
    
    // Found case
    result1 = binary_search_v1(single.data(), 1, 42);
    result2 = binary_search_v2(single.data(), 1, 42);
    result3 = binary_search_v3(single.data(), 1, 42);
    std::cout << "Single element found: V1=" << result1 << ", V2=" << result2 << ", V3=" << result3;
    std::cout << (result1 == 0 && result2 == 0 && result3 == 0 ? " - PASSED" : " - FAILED") << std::endl;
    
    // Not found case
    result1 = binary_search_v1(single.data(), 1, 99);
    result2 = binary_search_v2(single.data(), 1, 99);
    result3 = binary_search_v3(single.data(), 1, 99);
    std::cout << "Single element not found: V1=" << result1 << ", V2=" << result2 << ", V3=" << result3;
    std::cout << (result1 == -1 && result2 == -1 && result3 == -1 ? " - PASSED" : " - FAILED") << std::endl;
    
    // Test large values
    std::vector<int> large_vals = {1000000, 2000000, 3000000};
    result1 = binary_search_v1(large_vals.data(), 3, 2000000);
    result2 = binary_search_v2(large_vals.data(), 3, 2000000);
    result3 = binary_search_v3(large_vals.data(), 3, 2000000);
    std::cout << "Large values search: V1=" << result1 << ", V2=" << result2 << ", V3=" << result3;
    std::cout << (result1 == 1 && result2 == 1 && result3 == 1 ? " - PASSED" : " - FAILED") << std::endl;
}

int main() {
    try {
        print_system_info();
        test_correctness();
        test_edge_cases();
        measure_binary_search();        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 