#include "pch.h"

#include "../01_Beispiel/merge_sort.hpp"
#include "../01_Beispiel/random.cpp"
#include "../01_Beispiel/stream_reader.h"

TEST(RandomTest, ProducesIntValuesInRange) {
  for (int i = 0; i < 100; i++) {
    int val = random_int(1, 100);
    EXPECT_GE(val, 1);
    EXPECT_LE(val, 100);
  }
}

TEST(RandomTest, ProducesRealValuesInRange) {
  for (int i = 0; i < 100; i++) {
    double val = random_real();
    EXPECT_GE(val, 0.0);
    EXPECT_LE(val, 1.0);
  }
}

TEST(StreamReaderTest, TestReadAndPutback) {
  std::stringstream ss;
  ss.str("1 2 3 ");
  stream_reader<std::string> reader(ss);
  EXPECT_EQ(reader.get(), "1");
  ss.putback('4');
  EXPECT_EQ(reader.get(), "4");
  EXPECT_EQ(reader.get(), "2");
  EXPECT_EQ(reader.get(), "3");
  ASSERT_FALSE(reader.has_next());
}