#pragma once

#include <string>
#include <random>

/// <summary>
/// Generate a random int between and include min ... max.
/// </summary>
/// <param name="min">minimum value (inclusive)</param>
/// <param name="max">maximum value (inclusive)</param>
/// <returns>A random int r, with min <= r <= max</returns>
int random_int(const int min, const int max);

/// <summary>
/// Generate a uniformly distributed random double in range [0.0 ... 1.0)
/// </summary>
/// <returns>a double r, with 0.0 <= r < 1.0 </returns>
double random_real();

/// <summary>
/// Produce a random alphabetic character in the range 'a'..'z' or 'A'..'Z'.
/// </summary>
char random_char();

/// <summary>
/// Produce a random alphabetic string.
/// </summary>
/// <param name="len">length of random string</param>
std::string random_string(const int len = 4);
