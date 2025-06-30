#pragma once

#include <string>
#include <vector>

// A class to collect data about the performance of pattern search algorithms.
class data_collector {
public:
    // This struct is now public to allow for external aggregation.
    struct char_stats {
        size_t successful_comparisons = 0;
        size_t unsuccessful_comparisons = 0;
    };

    // Constructor: Initializes the collector for a text of a given size.
    data_collector(size_t text_size);

    // Records a character comparison.
    void record_comparison(size_t text_index, bool success);

    // Resets all collected data.
    void reset();

    // Writes the collected data to a CSV file.
    void write_to_csv(const std::string& filename) const;

    // Gets the total number of comparisons.
    size_t get_total_comparisons() const;

    // Gets the raw statistics for aggregation.
    const std::vector<char_stats>& get_text_statistics() const;

private:
    size_t total_comparisons_;
    std::vector<char_stats> text_statistics_;
}; 