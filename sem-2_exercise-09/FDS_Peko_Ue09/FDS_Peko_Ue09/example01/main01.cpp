#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <iomanip>
#include <functional>
#include "pfc-mini.hpp"
#include "system_info.cpp"
#include "timing_operations.cpp"


void measure_basic_operations() {
    std::cout << "\n=== Basic Operations Timing Measurement ===" << std::endl;
    
    const int iterations = 100'000'000;
    std::ofstream csv_file("../../../out/basic_operations_timing.csv");
    csv_file << "Operation,Time_ns,Iterations" << std::endl;
    
    // Initialize test array for indexing
    for (int i = 0; i < 1000000; ++i) {
        test_array[i] = i;
    }
    
	BasicOperationTimings timings = measure_all_basic_operations(iterations);

    // Measure and record each operation
    double add_ns = timings.add_ns;
    std::cout << "Add: " << std::fixed << std::setprecision(3) << add_ns << " ns" << std::endl;
    csv_file << "Add," << add_ns << "," << iterations << std::endl;
    
    double assign_ns = timings.assign_ns;
    std::cout << "Assign: " << std::fixed << std::setprecision(3) << assign_ns << " ns" << std::endl;
    csv_file << "Assign," << assign_ns << "," << iterations << std::endl;
    
    double compare_ns = timings.compare_ns;
    std::cout << "Compare: " << std::fixed << std::setprecision(3) << compare_ns << " ns" << std::endl;
    csv_file << "Compare," << compare_ns << "," << iterations << std::endl;
    
    double divide_ns = timings.divide_ns;
    std::cout << "Divide: " << std::fixed << std::setprecision(3) << divide_ns << " ns" << std::endl;
    csv_file << "Divide," << divide_ns << "," << iterations << std::endl;
    
    double multiply_ns = timings.multiply_ns;
    std::cout << "Multiply: " << std::fixed << std::setprecision(3) << multiply_ns << " ns" << std::endl;
    csv_file << "Multiply," << multiply_ns << "," << iterations << std::endl;
    
    double index_ns = timings.index_ns;
    std::cout << "Index: " << std::fixed << std::setprecision(3) << index_ns << " ns" << std::endl;
    csv_file << "Index," << index_ns << "," << iterations << std::endl;
    
    csv_file.close();
    std::cout << "Basic operations results saved to 'basic_operations_timing.csv'" << std::endl;
}

int main() {
    try {
        print_system_info();
        measure_basic_operations();        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}