#include <iostream>
#include <vector>
#include "heapsort.hpp"

long long count_cmp = 0;
long long count_swap = 0;

std::vector<int> generate_random_array(int size)
{
    std::vector<int> array(size);
    for (int i = 0; i < size; i++)
    {
        array[i] = rand() % size;
    }
    return array;
}

std::pair<long long, long long> run_for_size(int size, int iterations)
{
    // Reset counters
    count_cmp = 0;
    count_swap = 0;

    // Take iterations testsamples
    for (int i = 0; i < iterations; i++)
    {
        std::vector<int> array = generate_random_array(size);
        heap_sorter::sort(array);
    }

    // Calculate average
    auto average_comparisons = count_cmp / iterations;
    auto average_swaps = count_swap / iterations;

    // Print results
    std::cout << "Size: " << size << std::endl;
    std::cout << "Average comparisons: " << std::fixed << average_comparisons << std::endl;
    std::cout << "Average swaps: " << std::fixed << average_swaps << std::endl;
    std::cout << std::endl;

    return std::make_pair(average_comparisons, average_swaps);
}

void print_vector(const std::vector<long long>& vec)
{
    std::cout << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i != vec.size() - 1)
            std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

int main()
{
    int sizes[] = {100, 200, 500, 1000, 2000, 5000, 10000, 15000, 20000, 30000, 40000, 60000, 80000, 100000};
    std::vector<long long> compares;
    std::vector<long long> swaps;
    int iterations = 25;

    for (int size : sizes)
    {
        auto result = run_for_size(size, iterations);
        compares.push_back(result.first);
        swaps.push_back(result.second);
    }

    // Print results
    std::cout << "Compares: ";
    print_vector(compares);
    std::cout << "Swaps: ";
    print_vector(swaps);

    return 0;
}
