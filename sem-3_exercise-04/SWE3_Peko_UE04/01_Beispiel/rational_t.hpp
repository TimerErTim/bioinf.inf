#pragma once

#include <stdexcept>
#include <string>
#include <iosfwd>
#include <sstream>
#include <type_traits>
#include <utility>

// forward declare matrix_t for trait
namespace swe3 { template <typename> class matrix_t; }

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

// generic ops used by rational_t
#include "operations.h"

template <typename T>
class rational_t {
public:
	using value_type = T;

	rational_t() noexcept : numerator_(T{0}), denominator_(T{1}) {}
	explicit rational_t(const value_type& numerator) noexcept : numerator_(numerator), denominator_(T{1}) {}
	rational_t(const value_type& numerator, const value_type& denominator)
		: numerator_(numerator), denominator_(denominator) {
		if (ops::is_zero(denominator_)) {
			throw invalid_rational_error("denominator must not be zero");
		}
		normalize();
	}
	rational_t(const rational_t&) noexcept = default;
	rational_t& operator=(const rational_t& other) noexcept {
		if (this == &other) return *this;
		numerator_ = other.numerator_;
		denominator_ = other.denominator_;
		return *this;
	}

	value_type const& get_numerator() const noexcept { return numerator_; }
	value_type const& get_denominator() const noexcept { return denominator_; }

	bool is_negative() const noexcept { return ops::is_negative(numerator_); }
	bool is_positive() const noexcept { return !ops::is_zero(numerator_) && !ops::is_negative(numerator_); }
	bool is_zero() const noexcept { return ops::is_zero(numerator_); }

	void inverse() {
		if (ops::is_zero(numerator_)) {
			throw division_by_zero_error("cannot invert zero");
		}
		std::swap(numerator_, denominator_);
		normalize();
	}

	std::string as_string() const {
		std::ostringstream out;
		if (ops::equals(denominator_, T{1})) {
			out << numerator_;
		} else {
			out << numerator_ << '/' << denominator_;
		}
		return out.str();
	}

	// Compound assignment operators
	rational_t& operator+=(const rational_t& rhs) noexcept {
		numerator_ = numerator_ * rhs.denominator_ + rhs.numerator_ * denominator_;
		denominator_ = denominator_ * rhs.denominator_;
		normalize();
		return *this;
	}
	rational_t& operator-=(const rational_t& rhs) noexcept {
		numerator_ = numerator_ * rhs.denominator_ - rhs.numerator_ * denominator_;
		denominator_ = denominator_ * rhs.denominator_;
		normalize();
		return *this;
	}
	rational_t& operator*=(const rational_t& rhs) noexcept {
		numerator_ = numerator_ * rhs.numerator_;
		denominator_ = denominator_ * rhs.denominator_;
		normalize();
		return *this;
	}
	rational_t& operator/=(const rational_t& rhs) {
		if (rhs.is_zero()) {
			throw division_by_zero_error("division by zero rational number");
		}
		numerator_ = numerator_ * rhs.denominator_;
		denominator_ = denominator_ * rhs.numerator_;
		normalize();
		return *this;
	}

	// Binary arithmetic operators (delegating to compound) – Barton–Nackman as friends
	friend rational_t operator+(rational_t lhs, const rational_t& rhs) noexcept { lhs += rhs; return lhs; }
	friend rational_t operator-(rational_t lhs, const rational_t& rhs) noexcept { lhs -= rhs; return lhs; }
	friend rational_t operator*(rational_t lhs, const rational_t& rhs) noexcept { lhs *= rhs; return lhs; }
	friend rational_t operator/(rational_t lhs, const rational_t& rhs) { lhs /= rhs; return lhs; }

	// Comparisons
	friend bool operator==(const rational_t& a, const rational_t& b) noexcept {
		return ops::equals(a.numerator_, b.numerator_) && ops::equals(a.denominator_, b.denominator_);
	}
	friend bool operator!=(const rational_t& a, const rational_t& b) noexcept { return !(a == b); }
	friend bool operator<(const rational_t& a, const rational_t& b) noexcept {
		return (a.numerator_ * b.denominator_) < (b.numerator_ * a.denominator_);
	}
	friend bool operator>(const rational_t& a, const rational_t& b) noexcept { return b < a; }
	friend bool operator<=(const rational_t& a, const rational_t& b) noexcept { return !(b < a); }
	friend bool operator>=(const rational_t& a, const rational_t& b) noexcept { return !(a < b); }

	// Stream operators
	friend std::ostream& operator<<(std::ostream& os, const rational_t& r) {
		os << '<' << r.as_string() << '>';
		return os;
	}
	friend std::istream& operator>>(std::istream& is, rational_t& r) {
		T num{};
		T den{1};
		is >> std::ws;
		if (!(is >> num)) {
			return is;
		}
		if (is.peek() == '/') {
			is.get();
			if (!(is >> den)) {
				is.setstate(std::ios::failbit);
				return is;
			}
		}
		if (ops::is_zero(den)) {
			is.setstate(std::ios::failbit);
			throw invalid_rational_error("attempt to construct rational with zero denominator from stream");
		}
		r = rational_t(num, den);
		return is;
	}

private:
	void normalize() noexcept {
		// move sign to numerator if denominator is negative
		if (ops::is_negative(denominator_)) {
			numerator_ = ops::negate(numerator_);
			denominator_ = ops::negate(denominator_);
		}
		// Canonical zero: 0/x -> 0/1
		if (ops::is_zero(numerator_)) {
			denominator_ = T{1};
			return;
		}
		// Reduce if we know how
		if constexpr (std::is_same_v<T, int>) {
			auto g = ops::gcd(numerator_, denominator_);
			if (g != 0 && g != 1) {
				numerator_ /= g;
				denominator_ /= g;
			}
		} else if constexpr (std::is_same_v<T, matrix_t<typename T::value_type>>) {
			auto g = ops::gcd(numerator_, denominator_);
			if (!ops::equals(g, T{1})) {
				numerator_ = numerator_ / g;
				denominator_ = denominator_ / g;
			}
		}
	}

	value_type numerator_;
	value_type denominator_;
};

// Convenience overloads for mixing with int on the LHS for rational_t<int>
inline rational_t<int> operator+(int lhs, const rational_t<int>& rhs) noexcept { return rational_t<int>(lhs) + rhs; }
inline rational_t<int> operator-(int lhs, const rational_t<int>& rhs) noexcept { return rational_t<int>(lhs) - rhs; }
inline rational_t<int> operator*(int lhs, const rational_t<int>& rhs) noexcept { return rational_t<int>(lhs) * rhs; }
inline rational_t<int> operator/(int lhs, const rational_t<int>& rhs) { return rational_t<int>(lhs) / rhs; }

} // namespace swe3


