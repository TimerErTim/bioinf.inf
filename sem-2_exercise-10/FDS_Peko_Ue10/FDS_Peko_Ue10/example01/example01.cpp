#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <functional>
#include <iomanip>

#include "pattern_search.h"
#include "data_collector.h"

// Generates a random string of a given length from a given alphabet.
std::string generate_random_string(size_t length, const std::string& alphabet) {
    std::string result;
    result.reserve(length);
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<size_t> distribution(0, alphabet.length() - 1);

    for (size_t i = 0; i < length; ++i) {
        result += alphabet[distribution(generator)];
    }
    return result;
}

// A structure to hold the settings for a single test run.
struct test_case {
    std::string name;
    std::string alphabet;
    size_t text_length;
    size_t pattern_length;
};

// Runs a specific search algorithm and prints the results.
void run_search(
    const std::string& search_name,
    std::function<long long(const std::string&, const std::string&, data_collector&, size_t)> search_function,
    const std::string& text,
    const std::string& pattern,
    const std::string& test_name
) {
    std::cout << "--- Running " << search_name << " for " << test_name << " ---" << std::endl;
    data_collector collector(text.length());
    
    auto start_time = std::chrono::high_resolution_clock::now();
    long long position = search_function(text, pattern, collector, 0);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end_time - start_time;

    if (position != -1) {
        std::cout << "Pattern found at index: " << position << std::endl;
    } else {
        std::cout << "Pattern not found." << std::endl;
    }
    std::cout << "Total comparisons: " << collector.get_total_comparisons() << std::endl;
    std::cout << "Execution time: " << std::fixed << std::setprecision(4) << duration.count() << " ms" << std::endl;

    std::string filename = "out/" + test_name + "_" + search_name + ".csv";
    collector.write_to_csv(filename);
    std::cout << "Data written to " << filename << std::endl << std::endl;
}


int main() {
    const std::string binary_alphabet = "01";
    const std::string dna_alphabet = "ACGT";
    const std::string amino_acid_alphabet = "ACDEFGHIKLMNPQRSTVWY"; // 20 common, can be extended
    std::string ascii_alphabet;
    for (int i = 0; i < 256; ++i) {
        ascii_alphabet += static_cast<char>(i);
    }

    std::vector<test_case> test_cases = {
        {"binary_small", binary_alphabet, 100, 5},
        {"binary_large", binary_alphabet, 10000, 10},
        {"dna_small", dna_alphabet, 200, 10},
        {"dna_large", dna_alphabet, 20000, 20},
        {"amino_acid_small", amino_acid_alphabet, 500, 15},
        {"amino_acid_large", amino_acid_alphabet, 50000, 30},
        {"ascii_small", ascii_alphabet, 1000, 20},
        {"ascii_large", ascii_alphabet, 100000, 50}
    };

    for (const auto& tc : test_cases) {
        std::cout << "======================================================" << std::endl;
        std::cout << "Starting Test Scenario: " << tc.name << std::endl;
        std::cout << "======================================================" << std::endl;

        std::string text = generate_random_string(tc.text_length, tc.alphabet);
        std::string pattern = generate_random_string(tc.pattern_length, tc.alphabet);

        // To make sure the pattern is found sometimes, we can insert it randomly
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, text.length() - pattern.length());
        if (distrib(gen) % 2 == 0) { // 50% chance to insert pattern
             text.replace(distrib(gen), pattern.length(), pattern);
        }

        run_search("brute_force", brute_search::search, text, pattern, tc.name);
        run_search("boyer_moore", bm_search::search, text, pattern, tc.name);
        run_search("kmp", kmp_search::search, text, pattern, tc.name);
    }

    return 0;
}
