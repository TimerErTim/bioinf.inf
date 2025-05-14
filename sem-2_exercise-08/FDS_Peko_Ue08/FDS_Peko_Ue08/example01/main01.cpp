// example01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cassert>
#include "bstree.h"

void add_one(int& value) {
    value += 1;
}

void print_test_result(const std::string& test_name, bool result) {
    if (result) {
        std::cout << "Test " << test_name << ": " << "\033[32mPASSED\033[0m" << std::endl;
    } else {
        std::cout << "Test " << test_name << ": " << "\033[31mFAILED\033[0m" << std::endl;
    }
}

void run_empty_tree_tests() {
    std::cout << "=== Empty Tree Tests ===" << std::endl;
    
    bstree tree;
    bool test_result;
    
    // Test empty
    test_result = tree.empty();
    print_test_result("empty", test_result);
    
    // Test size
    test_result = tree.size() == 0;
    print_test_result("size", test_result);
    
    // Test contains
    test_result = !tree.contains(5);
    print_test_result("contains", test_result);
    
    // Test at
    int value;
    test_result = !tree.at(0, value);
    print_test_result("at", test_result);
    
    // Test remove
    test_result = !tree.remove(5);
    print_test_result("remove", test_result);
    
    // Test clear
    test_result = tree.clear() == 0;
    print_test_result("clear", test_result);
    
    std::cout << std::endl;
}

void run_insertion_tests() {
    std::cout << "=== Insertion Tests ===" << std::endl;
    
    bstree tree;
    bool test_result;
    
    // Insert values
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.insert(3);
    tree.insert(7);
    tree.insert(12);
    tree.insert(20);

    tree.print(std::cout) << std::endl;
    tree.print_2d(std::cout) << std::endl;
    tree.print_2d_upright(std::cout) << std::endl;
    
    // Test empty
    test_result = !tree.empty();
    print_test_result("not empty after insert", test_result);
    
    // Test size
    test_result = tree.size() == 7;
    print_test_result("size after insert", test_result);
    
    // Test contains
    test_result = tree.contains(10) && tree.contains(5) && tree.contains(15) &&
                 tree.contains(3) && tree.contains(7) && tree.contains(12) && 
                 tree.contains(20) && !tree.contains(100);
    print_test_result("contains after insert", test_result);
    
    // Print tree structure
    std::cout << "Tree in-order: ";
    tree.print(std::cout) << std::endl;
    
    std::cout << "Tree 2D representation:" << std::endl;
    tree.print_2d(std::cout) << std::endl;
    
    std::cout << "Tree 2D upright representation:" << std::endl;
    tree.print_2d_upright(std::cout) << std::endl;
    
    std::cout << std::endl;
}

void run_copy_constructor_tests() {
    std::cout << "=== Copy Constructor Tests ===" << std::endl;
    
    bstree tree1;
    tree1.insert(10);
    tree1.insert(5);
    tree1.insert(15);
    
    bstree tree2(tree1);
    
    bool test_result;
    
    // Test size
    test_result = tree2.size() == tree1.size();
    print_test_result("size after copy", test_result);
    
    // Test equals
    test_result = tree1.equals(tree2);
    print_test_result("equals after copy", test_result);
    
    // Test independence
    tree1.insert(20);
    test_result = !tree1.equals(tree2);
    print_test_result("independence after copy", test_result);
    
    std::cout << std::endl;
}

void run_removal_tests() {
    std::cout << "=== Removal Tests ===" << std::endl;
    
    bstree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.insert(3);
    tree.insert(7);
    tree.insert(12);
    tree.insert(20);
    
    bool test_result;
    
    // Test remove leaf node
    test_result = tree.remove(3);
    print_test_result("remove leaf node", test_result);
    test_result = !tree.contains(3) && tree.size() == 6;
    print_test_result("after removing leaf node", test_result);
    
    // Test remove node with one child
    test_result = tree.remove(5);
    print_test_result("remove node with one child", test_result);
    test_result = !tree.contains(5) && tree.contains(7) && tree.size() == 5;
    print_test_result("after removing node with one child", test_result);
    
    // Test remove node with two children
    test_result = tree.remove(15);
    print_test_result("remove node with two children", test_result);
    test_result = !tree.contains(15) && tree.contains(12) && tree.contains(20) && tree.size() == 4;
    print_test_result("after removing node with two children", test_result);
    
    // Test remove root
    test_result = tree.remove(10);
    print_test_result("remove root", test_result);
    test_result = !tree.contains(10) && tree.size() == 3;
    print_test_result("after removing root", test_result);
    
    // Test remove nonexistent value
    test_result = !tree.remove(100);
    print_test_result("remove nonexistent value", test_result);
    
    std::cout << std::endl;
}

void run_apply_tests() {
    std::cout << "=== Apply Tests ===" << std::endl;
    
    bstree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    
    // Apply add_one to all nodes
    tree.apply(add_one);
    
    bool test_result;
    
    // Verify values were changed
    test_result = tree.contains(11) && tree.contains(6) && tree.contains(16) && 
                 !tree.contains(10) && !tree.contains(5) && !tree.contains(15);
    print_test_result("apply function", test_result);
    
    std::cout << std::endl;
}

void run_at_tests() {
    std::cout << "=== At Tests ===" << std::endl;
    
    bstree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    
    bool test_result;
    int value;
    
    // Test valid indices
    test_result = tree.at(0, value) && value == 5;
    print_test_result("at index 0", test_result);
    
    test_result = tree.at(1, value) && value == 10;
    print_test_result("at index 1", test_result);
    
    test_result = tree.at(2, value) && value == 15;
    print_test_result("at index 2", test_result);
    
    // Test invalid indices
    test_result = !tree.at(-1, value);
    print_test_result("at negative index", test_result);
    
    test_result = !tree.at(3, value);
    print_test_result("at out of bounds index", test_result);
    
    std::cout << std::endl;
}

void run_count_tests() {
    std::cout << "=== Count Tests ===" << std::endl;
    
    bstree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(10);  // Duplicate
    tree.insert(15);
    tree.insert(10);  // Duplicate
    
    bool test_result;
    
    // Test count for existing values
    test_result = tree.count(10) == 3;
    print_test_result("count multiple occurrences", test_result);
    
    test_result = tree.count(5) == 1;
    print_test_result("count single occurrence", test_result);
    
    // Test count for nonexistent value
    test_result = tree.count(100) == 0;
    print_test_result("count nonexistent value", test_result);
    
    std::cout << std::endl;
}

void run_remove_all_tests() {
    std::cout << "=== Remove All Tests ===" << std::endl;
    
    bstree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(10);  // Duplicate
    tree.insert(15);
    tree.insert(10);  // Duplicate
    
    bool test_result;
    
    // Test remove_all for existing values
    test_result = tree.remove_all(10) == 3;
    print_test_result("remove_all multiple occurrences", test_result);
    test_result = !tree.contains(10) && tree.size() == 2;
    print_test_result("after remove_all multiple occurrences", test_result);
    
    test_result = tree.remove_all(5) == 1;
    print_test_result("remove_all single occurrence", test_result);
    test_result = !tree.contains(5) && tree.size() == 1;
    print_test_result("after remove_all single occurrence", test_result);
    
    // Test remove_all for nonexistent value
    test_result = tree.remove_all(100) == 0;
    print_test_result("remove_all nonexistent value", test_result);
    
    std::cout << std::endl;
}

void run_clear_tests() {
    std::cout << "=== Clear Tests ===" << std::endl;
    
    bstree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    
    bool test_result;
    
    // Test clear
    test_result = tree.clear() == 3;
    print_test_result("clear returns correct count", test_result);
    
    test_result = tree.empty() && tree.size() == 0;
    print_test_result("tree is empty after clear", test_result);
    
    std::cout << std::endl;
}

int main() {
    run_empty_tree_tests();
    run_insertion_tests();
    run_copy_constructor_tests();
    run_removal_tests();
    run_apply_tests();
    run_at_tests();
    run_count_tests();
    run_remove_all_tests();
    run_clear_tests();
    
    std::cout << "All tests completed." << std::endl;
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
