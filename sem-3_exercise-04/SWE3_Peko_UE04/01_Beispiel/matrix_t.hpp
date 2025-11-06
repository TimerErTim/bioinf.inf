#pragma once

// Lightweight 1×1 matrix wrapper used to demonstrate rationals over non-int domains.
// All operations reduce to operations on the single stored scalar value.

#include <ostream>
#include <istream>

template <typename T>
class matrix_t {
public:
	using value_type = T;

	matrix_t() : a11_{} {}
	explicit matrix_t(T const& v) : a11_{v} {}

	static matrix_t one() { return matrix_t{T{1}}; }
	static matrix_t zero() { return matrix_t{T{0}}; }

	T const& value() const noexcept { return a11_; }

	// Barton–Nackman style binary operators
	friend matrix_t operator+(matrix_t lhs, matrix_t const& rhs) { lhs.a11_ = lhs.a11_ + rhs.a11_; return lhs; }
	friend matrix_t operator-(matrix_t lhs, matrix_t const& rhs) { lhs.a11_ = lhs.a11_ - rhs.a11_; return lhs; }
	friend matrix_t operator*(matrix_t lhs, matrix_t const& rhs) { lhs.a11_ = lhs.a11_ * rhs.a11_; return lhs; }
	friend matrix_t operator/(matrix_t lhs, matrix_t const& rhs) { lhs.a11_ = lhs.a11_ / rhs.a11_; return lhs; }
	friend matrix_t operator%(matrix_t lhs, matrix_t const& rhs) { lhs.a11_ = lhs.a11_ % rhs.a11_; return lhs; }

	friend bool operator==(matrix_t const& a, matrix_t const& b) { return a.a11_ == b.a11_; }
	friend bool operator!=(matrix_t const& a, matrix_t const& b) { return !(a == b); }
	friend bool operator<(matrix_t const& a, matrix_t const& b) { return a.a11_ < b.a11_; }

	// unary minus
	friend matrix_t operator-(matrix_t const& m) {
		return matrix_t{ -m.a11_ };
	}

	friend std::ostream& operator<<(std::ostream& os, matrix_t const& m) {
		os << "[" << m.a11_ << "]";
		return os;
	}

	friend std::istream& operator>>(std::istream& is, matrix_t& m) {
		char ch1, ch2;
		T val;
		is >> ch1 >> val >> ch2;
		if (ch1 != '[' || ch2 != ']' ) {
			is.setstate(std::ios::failbit);
			return is;
		}
		m.a11_ = val;
		return is;
	}

private:
	T a11_;
};
