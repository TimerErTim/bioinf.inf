#pragma once

#include <concepts>
#include <iosfwd>
#include "operations.h"

template <typename T>
concept RationalElement = requires (T a, T b, std::ostream& os) {
	{ ops::is_zero(a) } -> std::convertible_to<bool>;
	{ ops::is_negative(a) } -> std::convertible_to<bool>;
	{ ops::negate(a) } -> std::same_as<T>;
	{ ops::equals(a, b) } -> std::convertible_to<bool>;
	{ ops::gcd(a, b) } -> std::same_as<T>;
	{ a + b } -> std::same_as<T>;
	{ a - b } -> std::same_as<T>;
	{ a * b } -> std::same_as<T>;
	{ a / b } -> std::same_as<T>;
	{ a < b } -> std::convertible_to<bool>;
	{ os << a } -> std::same_as<std::ostream&>;
	// construction from canonical constants used by rational_t
	{ T{0} };
	{ T{1} };
};



