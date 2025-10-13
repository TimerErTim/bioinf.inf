#include "pch.h"

#include "../01_Beispiel/merge_sort.hpp"
#include "../01_Beispiel/in_memory_io.cpp"
#include "../01_Beispiel/file_io.cpp"
#include "../01_Beispiel/random.cpp"
#include "../01_Beispiel/stream_reader.h"

constexpr int TEST_STRING_LENGTHS[] = {2, 10, 20};
constexpr int TEST_ARRAY_LENGTHS[] = {10, 100, 1000, 10000, 100000};

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

    std::vector<std::string> data;
    for (int i = 0; i < string_length; i++) {
        data.push_back(random_string(string_length));
    }

    
}
