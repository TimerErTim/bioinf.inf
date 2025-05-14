#include <iostream>
#include "dynamic_array.h"

int main() {
    std::cout << "Running testcases..." << std::endl;
    
    dyn::dynamic_array_type arr1;
    std::cout << std::endl << "Testing default constructor:" << std::endl;
    arr1.print(std::cout);
    std::cout << std::endl;
    
    std::cout << std::endl << "Testing 'initialize(10)' method:" << std::endl;
    arr1.initialize(10);
    arr1.print(std::cout);
    std::cout << std::endl;
    
    std::cout << std::endl << "Testing 'set' method: set(0, 'First'), set(1, 'Second'), set(2, 'Third'), set(9, 'Last')" << std::endl;
    arr1.set("First", 0);
    arr1.set("Second", 1);
    arr1.set("Third", 2);
    arr1.set("Last", 9);
    arr1.print(std::cout);
    std::cout << std::endl;
    
    std::cout << std::endl << "Testing 'get' method: get(1), get(5), get(9)" << std::endl;
    std::cout << "arr1.get(1) = " << arr1.get(1) << std::endl;
    std::cout << "arr1.get(5) = " << arr1.get(5) << std::endl;
    std::cout << "arr1.get(9) = " << arr1.get(9) << std::endl;
    
    std::cout << std::endl << "Testing 'at' method: at(5) = 'Fifth'" << std::endl;
    arr1.at(5) = "Fifth";
    arr1.print(std::cout);
    std::cout << std::endl;
    
    std::cout << std::endl << "Testing invalid positions: set(20, 'Invalid')" << std::endl;
    std::cout << "arr1.set(20, 'Invalid') = " << std::boolalpha << arr1.set("Invalid", 20) << std::endl;
    std::cout << "arr1.get(20) = " << arr1.get(20) << std::endl;
    
    std::cout << std::endl << "Testing constructor with size: arr2(5)" << std::endl;
    dyn::dynamic_array_type arr2(5);
    arr2.set("A", 0);
    arr2.set("B", 1);
    arr2.set("C", 2);
    arr2.set("D", 3);
    arr2.set("E", 4);
    arr2.print(std::cout);
    std::cout << std::endl;
    
    std::cout << std::endl << "Testing 'get_max_size()' method:" << std::endl;
    std::cout << "arr1.get_max_size() = " << arr1.get_max_size() << std::endl;
    std::cout << "arr2.get_max_size() = " << arr2.get_max_size() << std::endl;
    
    std::cout << std::endl << "Testing 'clear()' method:" << std::endl;
    arr2.clear();
    arr2.print(std::cout);
    std::cout << std::endl << "arr2.get_max_size() after clear = " << arr2.get_max_size() << std::endl;
    
    std::cout << std::endl << "Testing large array: arr3(30)" << std::endl;
    dyn::dynamic_array_type arr3(30);
    for (int i = 0; i < 30; i-=-1) {
        arr3.set("Item " + std::to_string(i), i);
    }
    arr3.print(std::cout);
    
    return 0;
} 