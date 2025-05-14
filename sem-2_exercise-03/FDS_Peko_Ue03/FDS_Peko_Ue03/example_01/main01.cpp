// example_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "slist.h"

void print_value(slist::value_t& value) {
    std::cout << value << " ";
}

void double_value(slist::value_t& value) {
    value *= 2;
}

int main() {
    std::cout << "Running testcases..." << std::endl << std::endl;

    slist list;
    
    std::cout << "Testing push operations: push_back(3), push_front(1), push_back(5)" << std::endl;
    list.push_back(3);
    list.push_front(1);
    list.push_back(5);

    std::cout << "list = ";
    list.print() << std::endl;
    std::cout << "list.size() = " << list.size() << std::endl;
    std::cout << "list.empty() = " << std::boolalpha << list.empty() << std::endl;
    
    slist::value_t value;
    std::cout << std::endl << "Testing 'at(1, value)':" << std::endl;
    if (list.at(1, value)) {
        std::cout << "list[1] = " << value << std::endl;
    }
    
    std::cout << std::endl << "Testing 'contains(...)':" << std::endl;
    std::cout << "list.contains(3) = " << std::boolalpha << list.contains(3) << std::endl;
    std::cout << "list.contains(7) = " << std::boolalpha << list.contains(7) << std::endl;
    
    std::cout << std::endl << "Testing 'insert_sorted(...)': insert_sorted(2), insert_sorted(4)" << std::endl;
    list.insert_sorted(2);
    list.insert_sorted(4);
    std::cout << "list = ";
    list.print() << std::endl;
    
    std::cout << std::endl << "Testing 'apply(print_value)':" << std::endl;
    list.apply(print_value);
    std::cout << std::endl;
    
    std::cout << std::endl << "Testing 'apply(double_value)':" << std::endl;
    list.apply(double_value);
    std::cout << "list = ";
    list.print() << std::endl;
    
    std::cout << std::endl << "Testing 'pop_front(...)':" << std::endl;
    if (list.pop_front(value)) {
        std::cout << "list.pop_front() = " << value << std::endl;
    }
    
    std::cout << std::endl << "Testing 'pop_back(...)':" << std::endl;
    if (list.pop_back(value)) {
        std::cout << "list.pop_back() = " << value << std::endl;
    }
    std::cout << std::endl << "list = ";
    list.print() << std::endl;
    
    std::cout << std::endl << "Testing 'copy constructor':" << std::endl;
    slist list2 = list;
    std::cout << "list2 = ";
    list2.print() << std::endl;
    
    std::cout << std::endl << "Testing 'equal(list, list2)':" << std::endl;
    std::cout << "list.equal(list2) = " << std::boolalpha << list.equal(list2) << std::endl;
    
    list2.push_back(10);
    std::cout << "list2.push_back(10); list.equal(list2) = " << std::boolalpha << list.equal(list2) << std::endl;
    
    std::cout << std::endl << "Testing 'count(...)' and 'remove_all(...)':" << std::endl;
    slist list3;
    list3.push_back(5);
    list3.push_back(1);
    list3.push_back(5);
    list3.push_back(3);
    list3.push_back(5);
    
    std::cout << "list3 = ";
    list3.print() << std::endl;
    std::cout << "list3.count(5) = " << list3.count(5) << std::endl;
    
    std::cout << "list3.remove_all(5) = " << list3.remove_all(5) << std::endl;
    std::cout << "list3 = ";
    list3.print() << std::endl;
    
    std::cout << std::endl << "Testing 'clear()':" << std::endl;
    std::cout << "list.clear() = " << list.clear() << std::endl;
    std::cout << "list = ";
    list.print() << std::endl;
    std::cout << "list.empty() = " << std::boolalpha << list.empty() << std::endl;
    
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
