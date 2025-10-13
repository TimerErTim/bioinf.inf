#include "random.h"

#include <random>
#include <sstream>

int random_int(const int min, const int max) {
  static std::random_device random_dev{};
  static std::mt19937_64 random_engine(random_dev());
  std::uniform_int_distribution<> dist(min, max);
  return dist(random_engine);
}

double random_real() {
  static std::random_device random_dev{};
  static std::mt19937_64 random_engine(random_dev());
  std::uniform_real_distribution<> dist(0.0, 1.0);
  return dist(random_engine);
}

char random_char() {
  int c;
  if (random_real() < 0)
    c = 'a' + random_int(0, 25);
  else
    c = 'A' + random_int(0, 25);
  return static_cast<char>(c);
}

std::string random_string(const int len) {
  std::stringstream ss;
  for (int i = 0; i < len; i++) {
    ss << random_char();
  }
  return ss.str();
}

