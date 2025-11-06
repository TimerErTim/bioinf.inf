#pragma once

// Lightweight 1×1 matrix wrapper used to demonstrate rationals over non-int domains.
// All operations reduce to operations on the single stored scalar value.
//
// Summary:
// - Minimal 1x1 matrix with scalar-like semantics.
// - Provides arithmetic operators and comparisons by delegating to the stored T.
// - Supports stream I/O in the form "[x]".

#include <ostream>
#include <istream>

template <typename T>
class matrix_t {
public:
	using value_type = T;

	/**
	 * @brief Default-constructs with value-initialized element.
	 */
	matrix_t() : a11_{} {}
	/**
	 * @brief Construct from scalar value.
	 * @param v scalar value for the single cell.
	 */
	explicit matrix_t(T const& v) : a11_{v} {}

	/** @brief Multiplicative identity [1]. */
	static matrix_t one() { return matrix_t{T{1}}; }
	/** @brief Additive identity [0]. */
	static matrix_t zero() { return matrix_t{T{0}}; }

	/**
	 * @brief Access stored value.
	 * @return const reference to the single element.
	 */
	T const& value() const noexcept { return a11_; }

	// Barton–Nackman style binary operators
	// Principle: Define as friends for symmetric resolution and potential inlining.
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

	/**
	 * @brief Write as "[x]".
	 */
	friend std::ostream& operator<<(std::ostream& os, matrix_t const& m) {
		os << "[" << m.a11_ << "]";
		return os;
	}

	/**
	 * @brief Read from format "[x]".
	 * @param is input stream
	 * @param m reference to matrix to fill
	 * @return reference to stream
	 */
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
