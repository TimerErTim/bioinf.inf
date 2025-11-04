#pragma once

#include <type_traits>
#include <cmath>

template <typename> class matrix_t;

namespace ops {

// Generic helpers with minimal assumptions. Overloads for int and matrix provided below.

template <typename T>
inline T abs(T const& a) { return a < T{0} ? -a : a; }

inline int abs(int a) { return std::abs(a); }

template <typename T>
inline bool equals(T const& a, T const& b) { return a == b; }

inline bool equals(int a, int b) { return a == b; }

template <typename T>
inline bool is_zero(T const& a) { return equals(a, T{0}); }

inline bool is_zero(int a) { return a == 0; }

template <typename T>
inline bool is_negative(T const& a) { return a < T{0}; }

inline bool is_negative(int a) { return a < 0; }

template <typename T>
inline T negate(T const& a) { return -a; }

inline int negate(int a) { return -a; }

// divides / remainder / gcd for int
inline bool divides(int a, int b) { return a != 0 ? (b % a == 0) : false; }
inline int remainder(int a, int b) { return a % b; }
inline int gcd(int a, int b) {
	a = std::abs(a);
	b = std::abs(b);
	while (b != 0) {
		const int t = a % b;
		a = b;
		b = t;
	}
	return a == 0 ? 1 : a;
}

// Forward declarations for matrix overloads
template <typename T>
bool divides(::matrix_t<T> const& a, ::matrix_t<T> const& b);

template <typename T>
::matrix_t<T> remainder(::matrix_t<T> const& a, ::matrix_t<T> const& b);

template <typename T>
::matrix_t<T> gcd(::matrix_t<T> a, ::matrix_t<T> b);

} // namespace ops


