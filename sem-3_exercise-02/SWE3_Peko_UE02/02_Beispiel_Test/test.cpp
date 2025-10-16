#include "pch.h"

#include "../02_Beispiel/merge_sort.cpp"
#include "../02_Beispiel/random.cpp" // Dont know why, but we have to import .cpp instead of .h for Test project to build
#include "../02_Beispiel/stream_reader.h"
#include "../02_Beispiel/file_manipulator.cpp"
#include <stdexcept>

constexpr int TEST_STRING_LENGTHS[] = {10, 100};
constexpr int TEST_ARRAY_LENGTHS[] = {100000, 1000000};

class MergeSorterTest: public testing::TestWithParam<std::tuple<int, int>> {};

INSTANTIATE_TEST_CASE_P(
    ParameterizedTests,
    MergeSorterTest, 
    testing::Combine(
        testing::ValuesIn(TEST_STRING_LENGTHS),
        testing::ValuesIn(TEST_ARRAY_LENGTHS)
    )
);

TEST_P(MergeSorterTest, TestSortOnDiskParameterized) {
    auto [string_length, array_length] = GetParam();

    // Arrange
    std::string filename = "on_disk_test_src_file.txt";
    file_manipulator::fill_randomly(filename, array_length, string_length);

    // Act
    merge_sorter sorter;
    sorter.sort_file_on_disk(filename);

    // Assert
    std::ifstream file(filename);
    stream_reader<std::string> reader(file);
    
    std::string prev = reader.get();
    while (reader.has_next()) {
        std::string current = reader.get();
        ASSERT_LE(prev, current) << "Elements are not in sorted order";
        prev = current;
    }
}

TEST(MergeSortTest, TestEmptyFileOnDisk) {
    // Arrange
    std::string filename = "empty_test_file_on_disk.txt";
    std::ofstream file(filename);
    file.close(); // Create empty file

    // Act
    merge_sorter sorter;
    sorter.sort_file_on_disk(filename);
    
    // Assert
    std::ifstream input_file(filename);
    stream_reader<std::string> reader(input_file);
    ASSERT_FALSE(reader.has_next()) << "Empty file should remain empty";

    // Clean up
    remove(filename.c_str());
}

TEST(MergeSortTest, TestReverseSortedFileOnDisk) {
    // Arrange
    std::string filename = "reverse_sorted_test_file_on_disk.txt";
    std::ofstream file(filename);
    file << "zebra ";
    file << "yellow ";
    file << "xray ";
    file << "whiskey ";
    file << "victor ";
    file << "delta ";
    file << "charlie ";
    file << "bravo ";
    file << "alpha ";
    file.close();

    // Act
    merge_sorter sorter;
    sorter.sort_file_on_disk(filename);
    
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

TEST(MergeSortTest, TestFileWithDuplicatesOnDisk) {
    // Arrange
    std::string filename = "duplicates_test_file_on_disk.txt";
    std::ofstream file(filename);
    file << "apple ";
    file << "banana ";
    file << "apple ";
    file << "cherry ";
    file << "banana ";
    file << "apple ";
    file.close();

    // Act
    merge_sorter sorter;
    sorter.sort_file_on_disk(filename);
    
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

TEST(MergeSortTest, TestFileWithNotOnlyAlphabeticOnDisk) {
    // Arrange
    std::string filename = "non_alphabetic_test_file_on_disk.txt";
    std::ofstream file(filename);
    file << "-1/24 ";
    file << "jb14oa& ";
    file << "8kla;1 ";
    file << "1sk0:3 ";
    file << "12.4 ";
    file << "1^&@*has ";
    file << "124ooa ";
    file << "jb14o ";
    file.close();

    // Act
    merge_sorter sorter;
    sorter.sort_file_on_disk(filename);
    
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

TEST(MergeSortTest, TestFileWithDifferentStringLengthsOnDisk) {
    // Arrange
    std::string filename = "different_lengths_test_file_on_disk.txt";
    std::ofstream file(filename);
    for (int i = 0; i < 100000; i++) {
        file << random_string(random_int(1, 100)) << " ";
    }
    file.close();

    // Act
    merge_sorter sorter;
    sorter.sort_file_on_disk(filename);
    
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

TEST(MergeSortTest, TestNonexistentFileOnDiskThrows) {
    // Arrange
    std::string filename = "__no_such_file_exists__.txt";
    remove(filename.c_str());

    // Act + Assert
    merge_sorter sorter;
    ASSERT_THROW(sorter.sort_file_on_disk(filename), std::runtime_error);
}

TEST(MergeSortTest, TestNonexistentFileInMemoryThrows) {
    // Arrange
    std::string filename = "__no_such_file_exists_in_memory__.txt";
    remove(filename.c_str());

    // Act + Assert
    merge_sorter sorter;
    ASSERT_THROW(sorter.sort_file_in_memory(filename), std::runtime_error);
}

