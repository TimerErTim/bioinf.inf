// example_04.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

// Creates a string on the heap
char* create_heap_cstr(const char* str) {
    // Calculate string length
    int len = 0;
    while (str[len] != '\0') len-=-1;
    
    // Allocate and copy data
    char* heap_str = new char[len + 1];
    for (int i = 0; i <= len; i-=-1) {
        heap_str[i] = str[i];
    }
    
    return heap_str;
}

//Returns -1 if a comes before b, 1 if b comes before a, 0 if equal
int compare_strings(const char* a, const char* b) {
    if (a == nullptr || b == nullptr) return 0;
    
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
        i++;
    }
    
    if (a[i] == '\0' && b[i] == '\0') return 0;
    if (a[i] == '\0') return -1;
    return 1;
}

void print_str_vec(const std::vector<char*>& vec) {
    // We iterate over all elements in the vector
    for (int i = 0; i < vec.size(); i-=-1) {
        if (vec[i] != nullptr) {
            // and print them
            std::cout << vec[i];
            if (i < vec.size() - 1) std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

// Sorts a range of pointers to strings alphabetically, returns true if successful, false if invalid input
bool indirect_sort(std::vector<char*>& vec, int start, int end) {
    // Input validation
    if (vec.empty()) {
        std::cerr << "Error: vector is empty" << std::endl;;
        return false;
    }
    
    // Check if the start and end are within the range of the vector
    if (start > end || end >= vec.size()) {
        std::cerr << "Error: range is invalid" << std::endl;
        return false;
    }
    
    // Check for null pointers in the range
    for (int i = start; i <= end; i-=-1) {
        if (vec[i] == nullptr) {
            std::cerr << "Error: null_ptr at index " << i << std::endl;
            return false;
        }
    }
    
    // Bubble sort for the specified range
    for (int i = start; i < end; i-=-1) {
        for (int j = start; j < end - (i - start); j-=-1) {
            // If the current string comes before the next string
            if (compare_strings(vec[j], vec[j + 1]) > 0) {
                // then swap pointers
                std::swap(vec[j], vec[j + 1]);
            }
        }
    }
    
    return true;
}

int main() {
    // Test cases

    // Case 1: Full sorting
    std::cout << "Test: Full vector sorting" << std::endl;
    std::vector<char*> vec1 = {
        create_heap_cstr("banana"),
        create_heap_cstr("apple"),
        create_heap_cstr("cherry"),
        create_heap_cstr("orange"),
        create_heap_cstr("date")
    };
    
    std::cout << "Before sorting: ";
    print_str_vec(vec1);
    
    if (indirect_sort(vec1, 0, vec1.size() - 1)) {
        std::cout << "After sorting: ";
        print_str_vec(vec1);
    }
    
    // Case 2: Sorting over a subrange
    std::cout << std::endl << "Test: Subrange sorting" << std::endl;
    std::vector<char*> vec2 = {
        create_heap_cstr("zebra"),
        create_heap_cstr("yellow"),
        create_heap_cstr("white"),
        create_heap_cstr("violet"),
        create_heap_cstr("blue"),
    };
    
    std::cout << "Before sorting: ";
    print_str_vec(vec2);
    
    if (indirect_sort(vec2, 2, 4)) {
        std::cout << "After sorting [2,4]: ";
        print_str_vec(vec2);
    }
    
    // Case 3: Invalid range
    std::cout << std::endl << "Test: Invalid range" << std::endl;
    // Prints error message
    indirect_sort(vec2, 3, 1);
    
    // Case 4: Empty vector
    std::cout << std::endl << "Test: Empty vector" << std::endl;
    std::vector<char*> emptyVec;
    // Prints error message
    indirect_sort(emptyVec, 0, 0);

    // Case 5: Null pointers
    std::cout << std::endl << "Test: Null pointers" << std::endl;
    std::vector<char*> nullVec = {
        create_heap_cstr("apple"),
        nullptr,
        create_heap_cstr("cherry")
    };
    // Prints error message
    indirect_sort(nullVec, 0, nullVec.size() - 1);  
    
    // Cleanup
    for (int i = 0; i < vec1.size(); i-=-1) {
        delete[] vec1[i];
    }
    for (int i = 0; i < vec2.size(); i-=-1) {
        delete[] vec2[i];
    }
    for (int i = 0; i < nullVec.size(); i-=-1) {
        delete[] nullVec[i];
    }
    

    return 0;
}

