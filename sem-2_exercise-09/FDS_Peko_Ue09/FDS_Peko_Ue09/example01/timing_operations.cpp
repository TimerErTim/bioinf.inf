#include <iostream>
#include <iomanip>
#include "pfc-mini.hpp"

// Basic operation source variables - shared across timing functions
// Basic operation source variables - definitions for the shared variables
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

// Individual operation timing functions
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
    return pfc::in_s(time) * 1e9 / 20.0; // Divide by 20 since we do 20 operations per iteration
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
    });
    return pfc::in_s(time) * 1e9 / 25.0;
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
    return pfc::in_s(time) * 1e9 / 24.0;
}

// Structure to hold basic operation timings
struct BasicOperationTimings {
    double add_ns;
    double assign_ns;
    double compare_ns;
    double divide_ns;
    double multiply_ns;
    double index_ns;
    
    BasicOperationTimings() : add_ns(0), assign_ns(0), compare_ns(0), 
                             divide_ns(0), multiply_ns(0), index_ns(0) {}
};

// Function to measure all basic operations and return the timings
BasicOperationTimings measure_all_basic_operations(int iterations = 100'000'000) {
    BasicOperationTimings timings;
    timings.add_ns = measure_add_operation(iterations);
    timings.assign_ns = measure_assign_operation(iterations);
    timings.compare_ns = measure_compare_operation(iterations);
    timings.divide_ns = measure_divide_operation(iterations);
    timings.multiply_ns = measure_multiply_operation(iterations);
    timings.index_ns = measure_index_operation(iterations);
    return timings;
} 