#include "pch.h"

#include "../01_Beispiel/merge_sort.cpp"
#include "../01_Beispiel/random.h"
#include "../01_Beispiel/stream_reader.h"
#include "../01_Beispiel/file_manipulator.cpp"

constexpr int TEST_STRING_LENGTHS[] = {2, 10, 20};
constexpr int TEST_ARRAY_LENGTHS[] = {10, 500, 10000};

class MergeSorterTest: public testing::TestWithParam<std::tuple<int, int>> {};

INSTANTIATE_TEST_CASE_P(
    ParameterizedTests,
    MergeSorterTest, 
    testing::Combine(
        testing::ValuesIn(TEST_STRING_LENGTHS),
        testing::ValuesIn(TEST_ARRAY_LENGTHS)
    )
);

TEST_P(MergeSorterTest, TestSortInMemoryParameterized) {
    auto [string_length, array_length] = GetParam();

    std::string filename = "test_file.txt";
    file_manipulator::fill_randomly(filename, array_length, string_length);

    merge_sorter sorter;
    sorter.sort_file_in_memory(filename);
    
    std::ifstream file(filename);
    stream_reader<std::string> reader(file);
    
    std::string prev = reader.get();
    while (reader.has_next()) {
        std::string current = reader.get();
        ASSERT_LE(prev, current) << "Elements are not in sorted order";
        prev = current;
    }

    remove(filename.c_str());
}
