#pragma once

#include <string>
#include <vector>
#include "data_collector.h"

// Provides a collection of static methods for pattern searching algorithms.
class brute_search {
public:
    // Searches for a pattern in a text using the brute-force algorithm.
    static long long search(
        const std::string& text,
        const std::string& pattern,
        data_collector& collector,
        size_t start_pos = 0);
};

class bm_search {
public:
    // Searches for a pattern in a text using the Boyer-Moore algorithm.
    static long long search(
        const std::string& text,
        const std::string& pattern,
        data_collector& collector,
        size_t start_pos = 0);
};

class kmp_search {
public:
    // Searches for a pattern in a text using the Knuth-Morris-Pratt algorithm.
    static long long search(
        const std::string& text,
        const std::string& pattern,
        data_collector& collector,
        size_t start_pos = 0);
}; 