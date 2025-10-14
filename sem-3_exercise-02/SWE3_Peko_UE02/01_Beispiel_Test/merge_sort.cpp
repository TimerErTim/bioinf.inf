#pragma once

#include "pch.h"

#include "../01_Beispiel/merge_sort.hpp"
#include "../01_Beispiel/random.h"
#include "../01_Beispiel/stream_reader.h"

TEST(MergeSortTest, TestEmptyFile) {
    // Arrange
    std::string filename = "empty_test_file.txt";
    std::ofstream file(filename);
    file.close(); // Create empty file

    // Act
    merge_sorter sorter;
    sorter.sort_file_in_memory(filename);
    
    // Assert
    std::ifstream input_file(filename);
    stream_reader<std::string> reader(input_file);
    ASSERT_FALSE(reader.has_next()) << "Empty file should remain empty";

    // Clean up
    remove(filename.c_str());
}

TEST(MergeSortTest, TestReverseSortedFile) {
    // Arrange
    std::string filename = "reverse_sorted_test_file.txt";
    std::ofstream file(filename);
    file << "zebra\n";
    file << "yellow\n";
    file << "xray\n";
    file << "whiskey\n";
    file << "victor\n";
    file.close();

    // Act
    merge_sorter sorter;
    sorter.sort_file_in_memory(filename);
    
    // Assert
    std::ifstream input_file(filename);
    stream_reader<std::string> reader(input_file);
    
    std::string prev = reader.get();
    while (reader.has_next()) {
        std::string current = reader.get();
        ASSERT_LE(prev, current) << "Elements are not in sorted order";
        prev = current;
    }

    // Clean up
    remove(filename.c_str());
}

TEST(MergeSortTest, TestFileWithDuplicates) {
    // Arrange
    std::string filename = "duplicates_test_file.txt";
    std::ofstream file(filename);
    file << "apple\n";
    file << "banana\n";
    file << "apple\n";
    file << "cherry\n";
    file << "banana\n";
    file << "apple\n";
    file.close();

    // Act
    merge_sorter sorter;
    sorter.sort_file_in_memory(filename);
    
    // Assert
    std::ifstream input_file(filename);
    stream_reader<std::string> reader(input_file);
    
    std::string prev = reader.get();
    while (reader.has_next()) {
        std::string current = reader.get();
        ASSERT_LE(prev, current) << "Elements are not in sorted order";
        prev = current;
    }

    // Clean up
    remove(filename.c_str());
}

TEST(MergeSortTest, TestFileWithNotOnlyAlphabetic) {
    // Arrange
    std::string filename = "duplicates_test_file.txt";
    std::ofstream file(filename);
    file << "-1/24\n";
    file << "jb14oa&\n";
    file << "8kla;1\n";
    file << "1sk0:3\n";
    file << "12.4\n";
    file << "1^&@*has\n";
    file << "124ooa\n";
    file << "jb14o\n";
    file.close();

    // Act
    merge_sorter sorter;
    sorter.sort_file_in_memory(filename);
    
    // Assert
    std::ifstream input_file(filename);
    stream_reader<std::string> reader(input_file);
    
    std::string prev = reader.get();
    while (reader.has_next()) {
        std::string current = reader.get();
        ASSERT_LE(prev, current) << "Elements are not in sorted order";
        prev = current;
    }

    // Clean up
    remove(filename.c_str());
}

TEST(MergeSortTest, TestFileWithDifferentStringLengths) {
    // Arrange
    std::string filename = "different_lengths_test_file.txt";
    std::ofstream file(filename);
    for (int i = 0; i < 10000; i++) {
        file << random_string(random_int(1, 100)) << " ";
    }
    file.close();

    // Act
    merge_sorter sorter;
    sorter.sort_file_in_memory(filename);
    
    // Assert
    std::ifstream input_file(filename);
    stream_reader<std::string> reader(input_file);
    
    std::string prev = reader.get();
    while (reader.has_next()) {
        std::string current = reader.get();
        ASSERT_LE(prev, current) << "Elements are not in sorted order";
        prev = current;
    }

    // Clean up
    remove(filename.c_str());
}

