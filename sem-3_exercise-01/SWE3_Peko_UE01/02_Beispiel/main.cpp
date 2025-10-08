#include <iostream>
#include <vector>
#include "heapsort.hpp"

void test_empty_array()
{
    heap_sorter::content_t data = {};
    heap_sorter::content_t expected = {};

    std::cout << "Sorting empty array..." << std::endl;
    heap_sorter::print_content(data);

    heap_sorter::sort(data);

    // Test assertion
    if (data == expected)
    {
        std::cout << "TEST PASSED: Empty array remains empty after sorting" << std::endl;
    }
    else
    {
        std::cout << "TEST FAILED: Empty array was modified during sorting" << std::endl;
    }
}

void test_negative_elements()
{
    heap_sorter::content_t data = {-5, 1, -10, -3, -8};
    heap_sorter::content_t expected = {-10, -8, -5, -3, 1};

    std::cout << "Sorting array with negative elements..." << std::endl;
    std::cout << "Original: ";
    heap_sorter::print_content(data);

    heap_sorter::sort(data);

    std::cout << "Sorted: ";
    heap_sorter::print_content(data);

    // Test assertion
    if (data == expected)
    {
        std::cout << "TEST PASSED: Negative elements sorted correctly" << std::endl;
    }
    else
    {
        std::cout << "TEST FAILED: Negative elements not sorted correctly" << std::endl;
    }
}

void test_descending_sorted_array()
{
    heap_sorter::content_t data = {10, 8, 6, 4, 2};
    heap_sorter::content_t expected = {2, 4, 6, 8, 10};

    std::cout << "Sorting descending sorted array..." << std::endl;
    std::cout << "Original: ";
    heap_sorter::print_content(data);

    heap_sorter::sort(data);

    std::cout << "Sorted: ";
    heap_sorter::print_content(data);

    // Test assertion
    if (data == expected)
    {
        std::cout << "TEST PASSED: Descending array sorted correctly" << std::endl;
    }
    else
    {
        std::cout << "TEST FAILED: Descending array not sorted correctly" << std::endl;
    }
}

void test_already_sorted_array()
{
    heap_sorter::content_t data = {2, 4, 6, 8, 10};
    heap_sorter::content_t expected = {2, 4, 6, 8, 10};

    std::cout << "Sorting already sorted array..." << std::endl;
    std::cout << "Original: ";
    heap_sorter::print_content(data);

    heap_sorter::sort(data);

    std::cout << "Sorted: ";
    heap_sorter::print_content(data);

    // Test assertion
    if (data == expected)
    {
        std::cout << "TEST PASSED: Already sorted array sorted correctly" << std::endl;
    }
    else
    {
        std::cout << "TEST FAILED: Already sorted array not sorted correctly" << std::endl;
    }
}

void test_duplicates()
{
    heap_sorter::content_t data = {5, 2, 8, 2, 9, 1, 5, 5};
    heap_sorter::content_t expected = {1, 2, 2, 5, 5, 5, 8, 9};

    std::cout << "Sorting array with duplicates..." << std::endl;
    std::cout << "Original: ";
    heap_sorter::print_content(data);

    heap_sorter::sort(data);

    std::cout << "Sorted: ";
    heap_sorter::print_content(data);

    // Test assertion
    if (data == expected)
    {
        std::cout << "TEST PASSED: Array with duplicates sorted correctly" << std::endl;
    }
    else
    {
        std::cout << "TEST FAILED: Array with duplicates not sorted correctly" << std::endl;
    }
}

void run_all_tests()
{
    std::cout << "=== Running Heapsort Test Cases ===" << std::endl;
    test_empty_array();
    test_negative_elements();
    test_descending_sorted_array();
    test_already_sorted_array();
    test_duplicates();
    std::cout << "\n=== All tests completed ===" << std::endl;
}

int main()
{
    //run_all_tests();
    test_duplicates();
    return 0;
}
