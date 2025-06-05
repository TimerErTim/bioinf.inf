#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <iomanip>
#include <functional>
#include "pfc-mini.hpp"
#include "system_info.cpp"

// Basic operation source variables
volatile int add_var1 = 1;
volatile int add_var2 = 2;
volatile int assign_var1 = 1;
volatile int assign_var2 = 2;
volatile int compare_var1 = 1;
volatile int compare_var2 = 2;
volatile double divide_var1 = 100.0;
volatile double divide_var2 = 3.0;
volatile double multiply_var1 = 3.0;
volatile double multiply_var2 = 4.0;
volatile int test_array[1000000];
volatile int index_result;

// Operation measurement functions
// Helper functions for each operation
double measure_add_operation(int iterations) {
    auto time = pfc::timed_run(iterations, []() {
        volatile int result = add_var1 + add_var2;
        (void)result;
        result = add_var1 + add_var2;
        (void)result;
        result = add_var1 + add_var2;
        (void)result;
        result = add_var1 + add_var2;
        (void)result;
        result = add_var1 + add_var2;
        (void)result;
        result = add_var1 + add_var2;
        (void)result;
        result = add_var1 + add_var2;
        (void)result;
        result = add_var1 + add_var2;
        (void)result;
        result = add_var1 + add_var2;
        (void)result;
        result = add_var1 + add_var2;
        (void)result;
		// 10 operations
		result = add_var1 + add_var2;
        (void)result;
        result = add_var1 + add_var2;
        (void)result;
        result = add_var1 + add_var2;
        (void)result;
        result = add_var1 + add_var2;
        (void)result;
        result = add_var1 + add_var2;
        (void)result;
        result = add_var1 + add_var2;
        (void)result;
        result = add_var1 + add_var2;
        (void)result;
        result = add_var1 + add_var2;
        (void)result;
        result = add_var1 + add_var2;
        (void)result;
        result = add_var1 + add_var2;
        (void)result;
		// 20 operations
    });
    return pfc::in_s(time) * 1e9 / 20.0; // Divide by 10 since we do 10 operations per iteration
}

double measure_assign_operation(int iterations) {
    auto time = pfc::timed_run(iterations, []() {
        assign_var1 = assign_var2;
        assign_var1 = assign_var2;
        assign_var1 = assign_var2;
        assign_var1 = assign_var2;
        assign_var1 = assign_var2;
        assign_var1 = assign_var2;
        assign_var1 = assign_var2;
        assign_var1 = assign_var2;
        assign_var1 = assign_var2;
        assign_var1 = assign_var2;
		// 10 operations
		assign_var1 = assign_var2;
        assign_var1 = assign_var2;
        assign_var1 = assign_var2;
        assign_var1 = assign_var2;
        assign_var1 = assign_var2;
        assign_var1 = assign_var2;
        assign_var1 = assign_var2;
        assign_var1 = assign_var2;
        assign_var1 = assign_var2;
        assign_var1 = assign_var2;
		// 20 operations
    });
    return pfc::in_s(time) * 1e9 / 20.0;
}

double measure_compare_operation(int iterations) {
    auto time = pfc::timed_run(iterations, []() {
        volatile bool result = compare_var1 < compare_var2;
        (void)result;
        result = compare_var1 < compare_var2;
        (void)result;
        result = compare_var1 < compare_var2;
        (void)result;
        result = compare_var1 < compare_var2;
        (void)result;
        result = compare_var1 < compare_var2;
        (void)result;
        result = compare_var1 < compare_var2;
        (void)result;
        result = compare_var1 < compare_var2;
        (void)result;
        result = compare_var1 < compare_var2;
        (void)result;
        result = compare_var1 < compare_var2;
        (void)result;
        result = compare_var1 < compare_var2;
        (void)result;
		// 10 operations
		result = compare_var1 < compare_var2;
        (void)result;
        result = compare_var1 < compare_var2;
        (void)result;
        result = compare_var1 < compare_var2;
        (void)result;
        result = compare_var1 < compare_var2;
        (void)result;
        result = compare_var1 < compare_var2;
        (void)result;
        result = compare_var1 < compare_var2;
        (void)result;
        result = compare_var1 < compare_var2;
        (void)result;
        result = compare_var1 < compare_var2;
        (void)result;
        result = compare_var1 < compare_var2;
        (void)result;
        result = compare_var1 < compare_var2;
        (void)result;
		// 20 operations
    });
    return pfc::in_s(time) * 1e9 / 20.0;
}

double measure_divide_operation(int iterations) {
    auto time = pfc::timed_run(iterations, []() {
        volatile double result = divide_var1 / divide_var2;
        (void)result;
        result = divide_var1 / divide_var2;
        (void)result;
        result = divide_var1 / divide_var2;
        (void)result;
        result = divide_var1 / divide_var2;
        (void)result;
        result = divide_var1 / divide_var2;
        (void)result;
        result = divide_var1 / divide_var2;
        (void)result;
        result = divide_var1 / divide_var2;
        (void)result;
        result = divide_var1 / divide_var2;
        (void)result;
        result = divide_var1 / divide_var2;
        (void)result;
        result = divide_var1 / divide_var2;
        (void)result;
		result = divide_var1 / divide_var2;
        (void)result;
        result = divide_var1 / divide_var2;
        (void)result;
        result = divide_var1 / divide_var2;
        (void)result;
        result = divide_var1 / divide_var2;
        (void)result;
        result = divide_var1 / divide_var2;
        (void)result;
        result = divide_var1 / divide_var2;
        (void)result;
        result = divide_var1 / divide_var2;
        (void)result;
        result = divide_var1 / divide_var2;
        (void)result;
        result = divide_var1 / divide_var2;
        (void)result;
        result = divide_var1 / divide_var2;
        (void)result;
    });
    return pfc::in_s(time) * 1e9 / 20.0;
}

double measure_multiply_operation(int iterations) {
    auto time = pfc::timed_run(iterations, []() {
        volatile double result = multiply_var1 * multiply_var2;
        (void)result;
        result = multiply_var1 * multiply_var2;
        (void)result;
        result = multiply_var1 * multiply_var2;
        (void)result;
        result = multiply_var1 * multiply_var2;
        (void)result;
        result = multiply_var1 * multiply_var2;
        (void)result;
        result = multiply_var1 * multiply_var2;
        (void)result;
        result = multiply_var1 * multiply_var2;
        (void)result;
        result = multiply_var1 * multiply_var2;
        (void)result;
        result = multiply_var1 * multiply_var2;
        (void)result;
        result = multiply_var1 * multiply_var2;
        (void)result;
		result = multiply_var1 * multiply_var2;
        (void)result;
		result = multiply_var1 * multiply_var2;
        (void)result;
		result = multiply_var1 * multiply_var2;
        (void)result;
		result = multiply_var1 * multiply_var2;
        (void)result;
		result = multiply_var1 * multiply_var2;
        (void)result;
		result = multiply_var1 * multiply_var2;
        (void)result;
		result = multiply_var1 * multiply_var2;
        (void)result;
		result = multiply_var1 * multiply_var2;
        (void)result;
		result = multiply_var1 * multiply_var2;
        (void)result;
		result = multiply_var1 * multiply_var2;
        (void)result;
		result = multiply_var1 * multiply_var2;
        (void)result;
		result = multiply_var1 * multiply_var2;
        (void)result;
		result = multiply_var1 * multiply_var2;
        (void)result;
		result = multiply_var1 * multiply_var2;
        (void)result;
		result = multiply_var1 * multiply_var2;
        (void)result;
    });
    return pfc::in_s(time) * 1e9 / 24.0;
}

double measure_index_operation(int iterations) {    
    auto time = pfc::timed_run(iterations, [&]() {
        index_result = test_array[2];	
		(void)index_result;
		index_result = test_array[2];
		(void)index_result;
		index_result = test_array[2];
		(void)index_result;
		index_result = test_array[2];
		(void)index_result;
		index_result = test_array[200];
		(void)index_result;
        index_result = test_array[1200];
		(void)index_result;
        index_result = test_array[2000];
		(void)index_result;
        index_result = test_array[5];
		(void)index_result;
        index_result = test_array[8];
		(void)index_result;
        index_result = test_array[1200];
		(void)index_result;
        index_result = test_array[2000];
		(void)index_result;
        index_result = test_array[5000];
		(void)index_result;
		index_result = test_array[2];	
		(void)index_result;
		index_result = test_array[2];
		(void)index_result;
		index_result = test_array[2];
		(void)index_result;
		index_result = test_array[2];
		(void)index_result;
		index_result = test_array[200];
		(void)index_result;
        index_result = test_array[1200];
		(void)index_result;
        index_result = test_array[2000];
		(void)index_result;
        index_result = test_array[5];
		(void)index_result;
        index_result = test_array[8];
		(void)index_result;
        index_result = test_array[1200];
		(void)index_result;
        index_result = test_array[2000];
		(void)index_result;
        index_result = test_array[5000];
		(void)index_result;
    });
    return pfc::in_s(time) * 1e9 / 20.0;
}

void measure_basic_operations() {
    std::cout << "\n=== Basic Operations Timing Measurement ===" << std::endl;
    
    const int iterations = 100'000'000;
    std::ofstream csv_file("../../../out/basic_operations_timing.csv");
    csv_file << "Operation,Time_ns,Iterations" << std::endl;
    
    // Initialize test array for indexing
    for (int i = 0; i < 1000000; ++i) {
        test_array[i] = i;
    }
    
    // Measure and record each operation
    double add_ns = measure_add_operation(iterations);
    std::cout << "Add: " << std::fixed << std::setprecision(3) << add_ns << " ns" << std::endl;
    csv_file << "Add," << add_ns << "," << iterations << std::endl;
    
    double assign_ns = measure_assign_operation(iterations);
    std::cout << "Assign: " << std::fixed << std::setprecision(3) << assign_ns << " ns" << std::endl;
    csv_file << "Assign," << assign_ns << "," << iterations << std::endl;
    
    double compare_ns = measure_compare_operation(iterations);
    std::cout << "Compare: " << std::fixed << std::setprecision(3) << compare_ns << " ns" << std::endl;
    csv_file << "Compare," << compare_ns << "," << iterations << std::endl;
    
    double divide_ns = measure_divide_operation(iterations);
    std::cout << "Divide: " << std::fixed << std::setprecision(3) << divide_ns << " ns" << std::endl;
    csv_file << "Divide," << divide_ns << "," << iterations << std::endl;
    
    double multiply_ns = measure_multiply_operation(iterations);
    std::cout << "Multiply: " << std::fixed << std::setprecision(3) << multiply_ns << " ns" << std::endl;
    csv_file << "Multiply," << multiply_ns << "," << iterations << std::endl;
    
    double index_ns = measure_index_operation(iterations);
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