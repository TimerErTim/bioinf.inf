#pragma once

#include <stdexcept>
#include <string>

// Summary:
// Custom exception types used by rational_t<T> to signal invalid construction
// and illegal arithmetic operations.

/**
 * @brief Thrown when attempting to construct a rational with denominator == 0
 *        or parse such a value from a stream.
 */
class invalid_rational_error : public std::invalid_argument {
public:
	explicit invalid_rational_error(const std::string& message)
		: std::invalid_argument(message) {}
};

/**
 * @brief Thrown when attempting to invert zero or divide by a zero rational.
 */
class division_by_zero_error : public std::domain_error {
public:
	explicit division_by_zero_error(const std::string& message)
		: std::domain_error(message) {}
};



