#pragma once

#include <stdexcept>
#include <string>
#include <iosfwd>

namespace swe3 {

class invalid_rational_error : public std::invalid_argument {
public:
	explicit invalid_rational_error(const std::string& message)
		: std::invalid_argument(message) {}
};

class division_by_zero_error : public std::domain_error {
public:
	explicit division_by_zero_error(const std::string& message)
		: std::domain_error(message) {}
};

class rational_t {
public:
	using value_type = int;

	rational_t() noexcept;
	rational_t(value_type numerator) noexcept;
	rational_t(value_type numerator, value_type denominator);
	rational_t(const rational_t&) noexcept;
	rational_t& operator=(const rational_t& other) noexcept;

	value_type get_numerator() const noexcept;
	value_type get_denominator() const noexcept;

	bool is_negative() const noexcept;
	bool is_positive() const noexcept;
	bool is_zero() const noexcept;

	std::string as_string() const;

	// Compound assignment operators
	rational_t& operator+=(const rational_t& rhs) noexcept;
	rational_t& operator-=(const rational_t& rhs) noexcept;
	rational_t& operator*=(const rational_t& rhs) noexcept;
	rational_t& operator/=(const rational_t& rhs);

	// Comparisons
	friend bool operator==(const rational_t& a, const rational_t& b) noexcept;
	friend bool operator!=(const rational_t& a, const rational_t& b) noexcept;
	friend bool operator<(const rational_t& a, const rational_t& b) noexcept;
	friend bool operator>(const rational_t& a, const rational_t& b) noexcept;
	friend bool operator<=(const rational_t& a, const rational_t& b) noexcept;
	friend bool operator>=(const rational_t& a, const rational_t& b) noexcept;

	// Binary arithmetic operators (delegating to compound)
	friend rational_t operator+(rational_t lhs, const rational_t& rhs) noexcept;
	friend rational_t operator-(rational_t lhs, const rational_t& rhs) noexcept;
	friend rational_t operator*(rational_t lhs, const rational_t& rhs) noexcept;
	friend rational_t operator/(rational_t lhs, const rational_t& rhs);

	// Stream operators
	friend std::ostream& operator<<(std::ostream& os, const rational_t& r);
	friend std::istream& operator>>(std::istream& is, rational_t& r);

private:
	static int gcd_int(int a, int b) noexcept;
	bool is_consistent() const noexcept;
	void normalize() noexcept;

	value_type numerator_;
	value_type denominator_;
};

// Convenience overloads with int to ensure symmetry without relying on implicit conversion on LHS
rational_t operator+(int lhs, const rational_t& rhs) noexcept;
rational_t operator-(int lhs, const rational_t& rhs) noexcept;
rational_t operator*(int lhs, const rational_t& rhs) noexcept;
rational_t operator/(int lhs, const rational_t& rhs);

} // namespace swe3


