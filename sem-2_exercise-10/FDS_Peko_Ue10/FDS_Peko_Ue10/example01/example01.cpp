#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <functional>
#include <iomanip>
#include <filesystem>
#include <numeric>
#include <algorithm>
#include <fstream>

#include "pattern_search.h"
#include "data_collector.h"
#include "pfc-mini.hpp"

// Generates a random string of a given length from a given alphabet.
std::string generate_random_string(size_t length, const std::string& alphabet) {
    std::string result;
    result.reserve(length);
    // Use a static generator for performance
    static std::mt19937 generator(std::random_device{}());
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

// Holds the results of a single search execution
struct run_result {
    long long position;
    pfc::default_duration duration;
    size_t comparisons;
    data_collector collector;
};

// Runs all test scenarios
void run_experiments(const std::vector<test_case>& test_cases);

// Runs a single scenario (e.g., binary_small) for all algorithms
void run_scenario(const test_case& tc);

// Runs a single search and returns its result
run_result run_single_search(
    std::function<long long(const std::string&, const std::string&, data_collector&, size_t)> search_function,
    const std::string& text,
    const std::string& pattern
);

// A struct to hold aggregated floating-point stats for the average case.
struct average_char_stats {
    double successful_comparisons = 0.0;
    double unsuccessful_comparisons = 0.0;
};

// Writes the calculated average case data to a CSV.
void write_average_data_to_csv(
    const std::string& filename,
    const std::vector<average_char_stats>& average_stats
) {
    std::ofstream out_file(filename);
    if (!out_file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return;
    }

    out_file << "text_index,successful_comparisons,unsuccessful_comparisons\n";
    for (size_t i = 0; i < average_stats.size(); ++i) {
        out_file << i << ","
                 << average_stats[i].successful_comparisons << ","
                 << average_stats[i].unsuccessful_comparisons << "\n";
    }
    out_file.close();
}

// Aggregates results from 100 runs and reports them
void aggregate_and_report(
    const std::string& search_name,
    const std::vector<run_result>& results,
    const std::string& base_filename
) {
    std::cout << "--- Aggregating for " << search_name << " ---" << std::endl;
    
    if (results.empty()) {
        std::cout << "No results to aggregate." << std::endl;
        return;
    }

    double total_duration_ms = 0;
    long long total_comparisons = 0;
    std::vector<average_char_stats> average_stats(results[0].collector.get_text_statistics().size());

    auto best_case_it = std::min_element(results.begin(), results.end(), 
        [](const auto& a, const auto& b) {
            return a.comparisons < b.comparisons;
        });

    auto worst_case_it = std::max_element(results.begin(), results.end(),
        [](const auto& a, const auto& b) {
            return a.comparisons < b.comparisons;
        });

    for(const auto& res : results) {
        total_duration_ms += pfc::in_s(res.duration) * 1000.0;
        total_comparisons += res.comparisons;
        
        const auto& stats = res.collector.get_text_statistics();
        for (size_t i = 0; i < stats.size(); ++i) {
            average_stats[i].successful_comparisons += stats[i].successful_comparisons;
            average_stats[i].unsuccessful_comparisons += stats[i].unsuccessful_comparisons;
        }
    }

    // Finalize averages
    for(auto& avg_stat : average_stats) {
        avg_stat.successful_comparisons /= results.size();
        avg_stat.unsuccessful_comparisons /= results.size();
    }

    std::cout << "Best case comparisons: " << best_case_it->comparisons << std::endl;
    std::cout << "Worst case comparisons: " << worst_case_it->comparisons << std::endl;
    std::cout << "Average comparisons: " << total_comparisons / results.size() << std::endl;
    std::cout << "Average execution time: " << std::fixed << std::setprecision(4) << total_duration_ms / results.size() << " ms" << std::endl;

    std::string best_case_filename = base_filename + "_best.csv";
    best_case_it->collector.write_to_csv(best_case_filename);
    std::cout << "Best case data written to " << best_case_filename << std::endl;

    std::string worst_case_filename = base_filename + "_worst.csv";
    worst_case_it->collector.write_to_csv(worst_case_filename);
    std::cout << "Worst case data written to " << worst_case_filename << std::endl;

    std::string average_case_filename = base_filename + "_average.csv";
    write_average_data_to_csv(average_case_filename, average_stats);
    std::cout << "Average case data written to " << average_case_filename << std::endl << std::endl;
}

int main() {
    const std::string binary_alphabet = "01";
    const std::string dna_alphabet = "ACGT";
    const std::string amino_acid_alphabet = "ACDEFGHIKLMNPQRSTVWY*XU"; // 23 common
    std::string ascii_alphabet;
    for (int i = 0; i < 256; ++i) {
        ascii_alphabet += static_cast<char>(i);
    }

    std::vector<test_case> test_cases = {
        {"binary_small", binary_alphabet, 1'000, 5},
        {"binary_large", binary_alphabet, 100'000, 10},
        {"dna_small", dna_alphabet, 2'000, 10},
        {"dna_large", dna_alphabet, 200'000, 20},
        {"amino_acid_small", amino_acid_alphabet, 5'000, 15},
        {"amino_acid_large", amino_acid_alphabet, 500'000, 30},
        {"ascii_small", ascii_alphabet, 10'000, 20},
        {"ascii_large", ascii_alphabet, 1'000'000, 50}
    };

    run_experiments(test_cases);
    return 0;
}

void run_experiments(const std::vector<test_case>& test_cases) {
    for (const auto& tc : test_cases) {
        std::cout << "======================================================" << std::endl;
        std::cout << "Starting Test Scenario: " << tc.name << std::endl;
        std::cout << "======================================================" << std::endl;
        run_scenario(tc);
    }
}

void run_scenario(const test_case& tc) {
    std::vector<run_result> brute_force_results;
    std::vector<run_result> boyer_moore_results;
    std::vector<run_result> kmp_results;
    
    brute_force_results.reserve(100);
    boyer_moore_results.reserve(100);
    kmp_results.reserve(100);
    
    // The main loop for 100 independent runs
    for (int i = 0; i < 100; ++i) {
        // Generate new text and pattern for each run
        std::string text = generate_random_string(tc.text_length, tc.alphabet);
        std::string pattern = generate_random_string(tc.pattern_length, tc.alphabet);
        
        // Insert pattern to guarantee a find
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, text.length() - pattern.length());
        text.replace(distrib(gen), pattern.length(), pattern);
        
        // Run all 3 methods on the same text and pattern
        brute_force_results.push_back(run_single_search(brute_search::search, text, pattern));
        boyer_moore_results.push_back(run_single_search(bm_search::search, text, pattern));
        kmp_results.push_back(run_single_search(kmp_search::search, text, pattern));
    }

    // Ensure the output directory exists
    std::filesystem::create_directories("../../../out");

    // Aggregate and report results for each algorithm
    aggregate_and_report("brute_force", brute_force_results, "../../../out/" + tc.name + "_brute_force");
    aggregate_and_report("boyer_moore", boyer_moore_results, "../../../out/" + tc.name + "_boyer_moore");
    aggregate_and_report("kmp", kmp_results, "../../../out/" + tc.name + "_kmp");
}

run_result run_single_search(
    std::function<long long(const std::string&, const std::string&, data_collector&, size_t)> search_function,
    const std::string& text,
    const std::string& pattern
) {
    data_collector collector(text.length());
    long long position = -1;
    auto duration = pfc::timed_run([&]() {
        position = search_function(text, pattern, collector, 0);
    });
    return {position, duration, collector.get_total_comparisons(), collector};
}
