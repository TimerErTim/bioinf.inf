#include "data_collector.h"
#include <fstream>
#include <iostream>

data_collector::data_collector(size_t text_size)
    : total_comparisons_(0), text_statistics_(text_size) {}

void data_collector::record_comparison(size_t text_index, bool success) {
    total_comparisons_++;
    if (text_index < text_statistics_.size()) {
        if (success) {
            text_statistics_[text_index].successful_comparisons++;
        } else {
            text_statistics_[text_index].unsuccessful_comparisons++;
        }
    }
}

void data_collector::reset() {
    total_comparisons_ = 0;
    for (auto& stats : text_statistics_) {
        stats.successful_comparisons = 0;
        stats.unsuccessful_comparisons = 0;
    }
}

void data_collector::write_to_csv(const std::string& filename) const {
    std::ofstream out_file(filename);
    if (!out_file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return;
    }

    out_file << "text_index,successful_comparisons,unsuccessful_comparisons\n";
    for (size_t i = 0; i < text_statistics_.size(); ++i) {
        out_file << i << ","
                 << text_statistics_[i].successful_comparisons << ","
                 << text_statistics_[i].unsuccessful_comparisons << "\n";
    }
    out_file.close();
}

size_t data_collector::get_total_comparisons() const {
    return total_comparisons_;
} 