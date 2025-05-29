#include <iostream>
#include <cassert>
#include <string>
#include "bstree.h"

void add_one(int& value) {
    value += 1;
}

void multiply_by_two(int& value) {
    value *= 2;
}

void print_test_header(const std::string& test_name) {
    std::cout << std::endl << "=== " << test_name << " ===" << std::endl;
}

void print_test_result(const std::string& test_name, bool result) {
    if (result) {
        std::cout << test_name << " -- PASSED" << std::endl;
    } else {
        std::cout << test_name << " -- FAILED" << std::endl;
    }
}

void print_tree_info(const bstree& tree, const std::string& name) {
    std::cout << name << " = ";
    tree.print(std::cout) << std::endl;
    std::cout << name << ".size() = " << tree.size() << std::endl;
}

void test_empty_tree() {
    print_test_header("Empty Tree");
    
    bstree tree;
    print_tree_info(tree, "tree");
    
    // Test basic properties
    print_test_result("tree.empty() returns true", tree.empty());
    print_test_result("tree.size() returns 0", tree.size() == 0);
    
    // Test operations on empty tree
    print_test_result("tree.contains(42) returns false", tree.contains(42) == false);
    print_test_result("tree.count(42) returns 0", tree.count(42) == 0);
    print_test_result("tree.remove(42) returns false", tree.remove(42) == false);
    print_test_result("tree.remove_all(42) returns 0", tree.remove_all(42) == 0);
    
    // Test at() with invalid indices
    int value;
    print_test_result("tree.at(-1, value) returns false", tree.at(-1, value) == false);
    print_test_result("tree.at(0, value) returns false", tree.at(0, value) == false);
    print_test_result("tree.at(1, value) returns false", tree.at(1, value) == false);
    
    // Test clear on empty tree
    print_test_result("tree.clear() returns 0", tree.clear() == 0);
    print_test_result("tree.empty() returns true", tree.empty());
    
    // Test apply on empty tree
    tree.apply(add_one);
    print_test_result("tree.apply(add_one) doesn't crash", true);
    
    // Test equals with another empty tree
    bstree tree2;
    print_tree_info(tree2, "tree2");
    print_test_result("tree equals tree2", tree.equals(tree2));
}

void test_single_node() {
    print_test_header("Single Node");
    
    bstree tree;
    tree.insert(42);
    print_tree_info(tree, "tree");
    
    // Test basic properties
    print_test_result("tree.empty() returns false", tree.empty() == false);
    print_test_result("tree.size() returns 1", tree.size() == 1);
    
    // Test contains
    print_test_result("tree.contains(42) returns true", tree.contains(42) == true);
    print_test_result("tree.contains(0) returns false", tree.contains(0) == false);
    print_test_result("tree.contains(100) returns false", tree.contains(100) == false);
    
    // Test count
    print_test_result("tree.count(42) returns 1", tree.count(42) == 1);
    print_test_result("tree.count(0) returns 0", tree.count(0) == 0);
    
    // Test at()
    int value;
    print_test_result("tree.at(0, value) returns true", tree.at(0, value));
    print_test_result("tree[0] is 42", value == 42);
    print_test_result("tree.at(-1, value) returns false", tree.at(-1, value) == false);
    print_test_result("tree.at(1, value) returns false", tree.at(1, value) == false);
    
    // Test remove
    print_test_result("tree.remove(0) returns false", tree.remove(0) == false);
    print_test_result("tree.size() still 1 after failed remove", tree.size() == 1);
    print_test_result("tree.remove(42) returns true", tree.remove(42) == true);
    print_test_result("tree.size() is 0 after remove", tree.size() == 0);
    print_test_result("tree.empty() returns true", tree.empty() == true);
    
    print_tree_info(tree, "tree");
}

void test_insertion_and_structure() {
    print_test_header("Insertion and Tree Structure");
    
    bstree tree;
    
    // Insert values to create a specific structure
    int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45};
    int num_values = sizeof(values) / sizeof(values[0]);
    
    for (int i = 0; i < num_values; i++) {
        tree.insert(values[i]);
        std::cout << "after inserting " << values[i] << ":" << std::endl;
        std::cout << "tree.size() = " << tree.size() << std::endl;
        std::cout << "tree.print_2d() = " << std::endl;
        tree.print_2d(std::cout) << std::endl;
        std::cout << "tree.print_2d_upright() = " << std::endl;
        tree.print_2d_upright(std::cout) << std::endl;
    }
    
    print_tree_info(tree, "tree");
    
    // Test size
    print_test_result("size() is correct", tree.size() == num_values);
    
    // Test contains for all inserted values
    bool all_contained = true;
    for (int i = 0; i < num_values; i++) {
        if (!tree.contains(values[i])) {
            all_contained = false;
            break;
        }
    }
    print_test_result("all inserted values are contained", all_contained);
    
    // Test at() for in-order traversal
    std::cout << "\ntesting at() method (in-order access):" << std::endl;
    for (int i = 0; i < tree.size(); i++) {
        int value;
        if (tree.at(i, value)) {
            std::cout << "tree[" << i << "] = " << value << std::endl;
        }
    }
    
    // Test duplicates
    std::cout << "\ntesting duplicates:" << std::endl;
    tree.insert(50); // Duplicate root
    tree.insert(30); // Duplicate left child
    print_tree_info(tree, "tree");
    print_test_result("size increased after duplicates", tree.size() == num_values + 2);
    print_test_result("count(50) returns 2", tree.count(50) == 2);
    print_test_result("count(30) returns 2", tree.count(30) == 2);
}

void test_copy_constructor() {
    print_test_header("Copy Constructor");
    
    bstree original;
    original.insert(10);
    original.insert(5);
    original.insert(15);
    original.insert(3);
    original.insert(7);
    
    print_tree_info(original, "original");
    
    // Test copy constructor
    bstree copy(original);
    print_tree_info(copy, "copy");
    
    print_test_result("copy.size() == original.size()", copy.size() == original.size());
    print_test_result("copy equals original", copy.equals(original));
    
    // Test independence
    original.insert(20);
    print_tree_info(original, "original after adding 20");
    print_tree_info(copy, "copy after original modification");
    
    print_test_result("copy.equals(original) returns false", copy.equals(original) == false);
    print_test_result("copy.size() == 5", copy.size() == 5);
    print_test_result("original.size() == 6", original.size() == 6);
    
    // Test copying empty tree
    bstree empty_tree;
    bstree empty_copy(empty_tree);
    print_test_result("empty_copy.empty() returns true", empty_copy.empty() == true);
    print_test_result("empty_tree.equals(empty_copy) returns true", empty_tree.equals(empty_copy) == true);
}

void test_removal_edge_cases() {
    print_test_header("Removal Edge Cases");
    
    // Test removing leaf nodes
    bstree tree1;
    tree1.insert(10);
    tree1.insert(5);
    tree1.insert(15);
    print_tree_info(tree1, "tree1");
    
    print_test_result("tree1.remove(5) returns true", tree1.remove(5));
    print_tree_info(tree1, "tree1");
    print_test_result("tree1.size() == 2", tree1.size() == 2);
    print_test_result("tree1.contains(5) returns false", tree1.contains(5) == false);
    
    // Test removing node with one child
    bstree tree2;
    tree2.insert(10);
    tree2.insert(5);
    tree2.insert(3);
    print_tree_info(tree2, "tree2");
    
    print_test_result("tree2.remove(5) returns true", tree2.remove(5));
    print_tree_info(tree2, "tree2");
    print_test_result("tree2.contains(3) returns true", tree2.contains(3) == true);
    
    // Test removing node with two children
    bstree tree3;
    tree3.insert(10);
    tree3.insert(5);
    tree3.insert(15);
    tree3.insert(3);
    tree3.insert(7);
    tree3.insert(12);
    tree3.insert(20);
    print_tree_info(tree3, "tree3");
    
    print_test_result("tree3.remove(15) returns true", tree3.remove(15));
    print_tree_info(tree3, "tree3");
    print_test_result("tree3.contains(15) returns false", tree3.contains(15) == false);
    print_test_result("all other nodes still contained", 
                     tree3.contains(10) && tree3.contains(5) && tree3.contains(3) && 
                     tree3.contains(7) && tree3.contains(12) && tree3.contains(20));
    
    // Test removing root
    bstree tree4;
    tree4.insert(10);
    tree4.insert(5);
    tree4.insert(15);
    print_tree_info(tree4, "tree4");
    
    print_test_result("tree4.remove(10) returns true", tree4.remove(10));
    print_tree_info(tree4, "tree4");
    print_test_result("tree4.contains(10) returns false", tree4.contains(10) == false);
    print_test_result("other nodes still contained", tree4.contains(5) && tree4.contains(15));
    
    // Test removing all nodes one by one
    bstree tree5;
    tree5.insert(10);
    tree5.insert(5);
    tree5.insert(15);
    
    while (!tree5.empty()) {
        int size_before = tree5.size();
        int value;
        tree5.at(0, value); // Get first value in in-order traversal
        tree5.remove(value);
        std::cout << "after removing " << value << ":" << std::endl;
        std::cout << "tree5.size() = " << tree5.size() << std::endl;
        tree5.print_2d(std::cout) << std::endl;
        print_test_result("size decreased", tree5.size() == size_before - 1);
    }
    print_test_result("tree5 is empty after removing all", tree5.empty());
}

void test_remove_all() {
    print_test_header("Remove All");
    
    bstree tree;
    
    // Insert multiple duplicates
    tree.insert(10);
    tree.insert(5);
    tree.insert(10);
    tree.insert(15);
    tree.insert(10);
    tree.insert(5);
    tree.insert(10);
    
    print_tree_info(tree, "tree");
    std::cout << "tree.count(10) = " << tree.count(10) << std::endl;
    std::cout << "tree.count(5) = " << tree.count(5) << std::endl;
    
    // Remove all 10s
    int removed_count = tree.remove_all(10);
    print_test_result("tree.remove_all(10) returned 4", removed_count == 4);
    print_test_result("tree.contains(10) returns false", !tree.contains(10));
    print_test_result("tree.count(10) is 0", tree.count(10) == 0);
    print_test_result("tree.contains(5) returns true", tree.contains(5));
    
    // Remove all 5s
    removed_count = tree.remove_all(5);
    print_tree_info(tree, "tree");
    print_test_result("tree.remove_all(5) returned 2", removed_count == 2);
    print_test_result("tree.contains(5) returns false", !tree.contains(5));
    
    // Remove all from non-existent value
    removed_count = tree.remove_all(100);
    print_test_result("tree.remove_all(100) returned 0", removed_count == 0);
    
    // Test remove_all on single occurrence
    tree.insert(42);
    print_tree_info(tree, "tree");
    removed_count = tree.remove_all(42);
    print_test_result("tree.remove_all(42) returned 1", removed_count == 1);
    print_test_result("tree.contains(42) returns false", !tree.contains(42));
}

void test_apply_function() {
    print_test_header("Apply Function");
    
    bstree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.insert(3);
    tree.insert(7);
    
    print_tree_info(tree, "tree");
    
    // Apply add_one function
    tree.apply(add_one);
    std::cout << "after applying add_one (x => x + 1):" << std::endl;
    print_tree_info(tree, "tree");
    
    // Verify all values were incremented
    print_test_result("tree.contains(11) returns true", tree.contains(11));
    print_test_result("tree.contains(6) returns true", tree.contains(6));
    print_test_result("tree.contains(16) returns true", tree.contains(16));
    print_test_result("tree.contains(4) returns true", tree.contains(4));
    print_test_result("tree.contains(8) returns true", tree.contains(8));
    
    // Verify old values are gone
    print_test_result("tree.contains(10) returns false", !tree.contains(10));
    print_test_result("tree.contains(5) returns false", !tree.contains(5));
    
    // Apply multiply_by_two function
    tree.apply(multiply_by_two);
    std::cout << "after applying multiply_by_two (x => x * 2):" << std::endl;
    print_tree_info(tree, "tree");
    
    // Test apply on empty tree
    bstree empty_tree;
    empty_tree.apply(add_one);
    print_test_result("apply on empty tree doesn't crash", empty_tree.empty());
}

void test_at_method_comprehensive() {
    print_test_header("Comprehensive at() Method");
    
    bstree tree;
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    int num_values = sizeof(values) / sizeof(values[0]);
    
    for (int i = 0; i < num_values; i++) {
        tree.insert(values[i]);
    }
    
    print_tree_info(tree, "tree");
    
    // Test all valid indices
    std::cout << "\ntesting all valid indices:" << std::endl;
    for (int i = 0; i < tree.size(); i++) {
        int value;
        bool success = tree.at(i, value);
        std::cout << "tree[" << i << "] = " << value << " (success: " << success << ")" << std::endl;
        print_test_result("tree.at(" + std::to_string(i) + ") succeeds", success);
    }
    
    // Test invalid indices
    int value;
    print_test_result("tree.at(-1) fails", tree.at(-1, value) == false);
    print_test_result("tree.at(-100) fails", tree.at(-100, value) == false);
    print_test_result("tree.at(size) fails", tree.at(tree.size(), value) == false);
    print_test_result("tree.at(size+1) fails", tree.at(tree.size() + 1, value) == false);
    print_test_result("tree.at(1000) fails", tree.at(1000, value) == false);
    
    // Verify in-order property
    std::cout << "\nverifying in-order property:" << std::endl;
    bool in_order = true;
    int prev_value = -1000000;
    for (int i = 0; i < tree.size(); i++) {
        int current_value;
        tree.at(i, current_value);
        if (current_value <= prev_value) {
            in_order = false;
            break;
        }
        prev_value = current_value;
    }
    print_test_result("values are in ascending order", in_order);
}

void test_clear_method() {
    print_test_header("Clear Method");
    
    // Test clear on non-empty tree
    bstree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.insert(3);
    tree.insert(7);
    
    print_tree_info(tree, "tree");
    int original_size = tree.size();
    
    int cleared_count = tree.clear();
    print_tree_info(tree, "tree_after_clear");
    
    print_test_result("tree.clear() returned original size", cleared_count == original_size);
    print_test_result("tree is empty after clear", tree.empty());
    print_test_result("tree.size() is 0 after clear", tree.size() == 0);
    
    // Test operations on cleared tree
    print_test_result("tree.contains(10) returns false after clear", tree.contains(10) == false);
    print_test_result("tree.remove(10) returns false after clear", tree.remove(10) == false);
    
    // Test clear on already empty tree
    int second_clear = tree.clear();
    print_test_result("second clear() returns 0", second_clear == 0);
    print_test_result("tree is still empty after second clear", tree.empty());
}

void test_equals_method() {
    print_test_header("Equals Method");
    
    // Test identical trees
    bstree tree1, tree2;
    int values[] = {10, 5, 15, 3, 7};
    int num_values = sizeof(values) / sizeof(values[0]);
    
    for (int i = 0; i < num_values; i++) {
        tree1.insert(values[i]);
        tree2.insert(values[i]);
    }
    
    print_tree_info(tree1, "tree1");
    print_tree_info(tree2, "tree2");
    print_test_result("identical trees are equal", tree1.equals(tree2));
    print_test_result("equals is symmetric (t2 == t1 is t1 == t2)", tree2.equals(tree1));
    
    // Test different sizes
    tree2.insert(20);
    print_tree_info(tree2, "tree2");
    print_test_result("trees with different sizes are not equal", tree1.equals(tree2) == false);
    
    // Test same size, different structure
    bstree tree3;
    tree3.insert(15);
    tree3.insert(10);
    tree3.insert(5);
    tree3.insert(7);
    tree3.insert(3);
    
    print_tree_info(tree3, "tree3");
    print_test_result("same values different structure are not equal", tree1.equals(tree3) == false);
    
    // Test with duplicates
    bstree tree4, tree5;
    tree4.insert(10);
    tree4.insert(10);
    tree5.insert(10);
    tree5.insert(10);
    
    print_tree_info(tree4, "tree4");
    print_tree_info(tree5, "tree5");
    print_test_result("trees with same duplicates are equal", tree4.equals(tree5));
    
    tree5.insert(10);
    print_tree_info(tree5, "tree5");
    print_test_result("trees with different duplicate counts are not equal", tree4.equals(tree5) == false);
    
    // Test empty trees
    bstree empty1, empty2;
    print_test_result("empty trees are equal", empty1.equals(empty2));
    
    // Test empty vs non-empty
    print_test_result("empty tree not equal to non-empty", !empty1.equals(tree1));
}

void test_extreme_cases() {
    print_test_header("Extreme Cases");
    
    // Test with negative numbers
    bstree neg_tree;
    neg_tree.insert(-10);
    neg_tree.insert(-20);
    neg_tree.insert(-5);
    neg_tree.insert(-15);
    neg_tree.insert(-1);
    
    print_tree_info(neg_tree, "neg_tree");
    print_test_result("negative numbers work correctly", 
                     neg_tree.contains(-10) && neg_tree.contains(-20) && neg_tree.size() == 5);
    
    // Test with zero
    bstree zero_tree;
    zero_tree.insert(0);
    zero_tree.insert(-1);
    zero_tree.insert(1);
    
    print_tree_info(zero_tree, "zero_tree");
    print_test_result("zero works correctly", zero_tree.contains(0) && zero_tree.size() == 3);
    
    // Test large numbers
    bstree large_tree;
    large_tree.insert(1000000);
    large_tree.insert(999999);
    large_tree.insert(1000001);
    
    print_tree_info(large_tree, "large_tree");
    print_test_result("large numbers work correctly", 
                     large_tree.contains(1000000) && large_tree.size() == 3);
    
    // Test many duplicates
    bstree dup_tree;
    for (int i = 0; i < 10; i++) {
        dup_tree.insert(42);
    }
    
    print_tree_info(dup_tree, "dup_tree");
    print_test_result("many duplicates work correctly", 
                     dup_tree.count(42) == 10 && dup_tree.size() == 10);
    
    // Test degenerate tree (essentially a linked list)
    bstree degen_tree;
    for (int i = 1; i <= 10; i++) {
        degen_tree.insert(i);
    }
    
    print_tree_info(degen_tree, "degen_tree");
    print_test_result("degenerate tree works correctly", degen_tree.size() == 10);
    
    // Test reverse degenerate tree
    bstree rev_degen_tree;
    for (int i = 10; i >= 1; i--) {
        rev_degen_tree.insert(i);
    }
    
    print_tree_info(rev_degen_tree, "rev_degen_tree");
    print_test_result("reverse degenerate tree works correctly", rev_degen_tree.size() == 10);
}

int main() {
    std::cout << "Binary Search Tree Test Suite" << std::endl;
    std::cout << "=============================" << std::endl;
    
    test_empty_tree();
    test_single_node();
    test_insertion_and_structure();
    test_copy_constructor();
    test_removal_edge_cases();
    test_remove_all();
    test_apply_function();
    test_at_method_comprehensive();
    test_clear_method();
    test_equals_method();
    test_extreme_cases();
    
    std::cout << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "All tests completed!" << std::endl;
    
    return 0;
} 