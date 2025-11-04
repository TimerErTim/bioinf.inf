#pragma once

#include <iosfwd>
#include <stdexcept>

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

	friend bool operator==(matrix_t const& a, matrix_t const& b) { return a.a11_ == b.a11_; }
	friend bool operator!=(matrix_t const& a, matrix_t const& b) { return !(a == b); }
	friend bool operator<(matrix_t const& a, matrix_t const& b) { return a.a11_ < b.a11_; }

	friend std::ostream& operator<<(std::ostream& os, matrix_t const& m) {
		os << "[" << m.a11_ << "]";
		return os;
	}

private:
	T a11_;
};

// ops specializations for matrix_t live here to avoid circular includes
#include "operations.h"

namespace ops {

template <typename T>
inline ::matrix_t<T> abs(::matrix_t<T> const& a) { return ::matrix_t<T>{ a.value() < T{0} ? -a.value() : a.value() }; }

template <typename T>
inline bool equals(::matrix_t<T> const& a, ::matrix_t<T> const& b) { return a == b; }

template <typename T>
inline bool is_zero(::matrix_t<T> const& a) { return a == ::matrix_t<T>::zero(); }

template <typename T>
inline bool is_negative(::matrix_t<T> const& a) { return a.value() < T{0}; }

template <typename T>
inline ::matrix_t<T> negate(::matrix_t<T> const& a) { return ::matrix_t<T>{ -a.value() }; }

template <typename T>
inline bool divides(::matrix_t<T> const& a, ::matrix_t<T> const& b) {
	// In a 1x1 matrix ring over a field-like T, a divides b iff a != 0
	return !is_zero(a);
}

template <typename T>
inline ::matrix_t<T> remainder(::matrix_t<T> const& /*a*/, ::matrix_t<T> const& /*b*/) {
	// Euclidean remainder is always zero for our 1x1 field-like matrices when using gcd fallback
	return ::matrix_t<T>::zero();
}

template <typename T>
inline ::matrix_t<T> gcd(::matrix_t<T> a, ::matrix_t<T> b) {
	// For 1x1 matrices over a field-like T, any non-zero element is a unit; normalize to 1
	(void)a; (void)b;
	return ::matrix_t<T>::one();
}

} // namespace ops


