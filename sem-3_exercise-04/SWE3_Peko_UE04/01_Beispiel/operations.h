#pragma once

#include <type_traits>
#include <limits>

// Generic operations abstraction for minimally constraining T used in rational_t<T>.
// Provide overloads for int, and allow other domains to add overloads as needed.
//
// Summary:
// - Defines minimal operations that can be specialized per domain.
// - Includes reasonable defaults where possible and full overloads for int.

namespace ops {

/**
 * @brief Equality predicate.
 * @tparam T element type
 * @param a left operand
 * @param b right operand
 * @return true if a == b
 */
// equals
template <typename T>
inline bool equals(T const &a, T const &b) {
  return a == b;
}

/** @brief Zero test: a == T{0}. */
// is_zero
template <typename T>
inline bool is_zero(T const &a) {
  return equals(a, T{0});
}

/** @brief Negativity test: a < T{0}. */
// is_negative
template <typename T>
inline bool is_negative(T const &a) {
  return a < T{0};
}

/** @brief Unary negation using 0 - a. */
// negate
template <typename T>
inline T negate(T const &a) {
  return T{0} - a;
}

/** @brief Absolute value using is_negative and negate. */
// abs (generic, relies on comparisons and negate)
template <typename T>
inline T abs(T const &a) {
  return is_negative(a) ? negate(a) : a;
}

// remainder (generic fallback disabled)
// Provide for int; other domains can overload.

/**
 * @brief Divisibility test. Generic default only treats ±1 as divisors.
 * @tparam T element type
 * @param a potential divisor
 * @param b dividend
 * @return true if a divides b (conservative default)
 */
// divides (generic, uses remainder)
template <typename T>
inline bool divides(T const &a, T const &b) {
  // In absence of a meaningful remainder, conservatively say false unless a is 1 or -1.
  // Domains should provide a stronger overload when available.
  return equals(abs(a), T{1});
}

// gcd (generic Euclidean algorithm; relies on remainder). Domains without remainder
// should overload this to return a multiplicative identity (e.g., one()) if appropriate.
/**
 * @brief Greatest common divisor using Euclidean algorithm.
 * @tparam T element type supporting ops::remainder
 * @param a first value (by copy)
 * @param b second value (by copy)
 * @return gcd(a,b)
 */
template <typename T>
inline T gcd(T a, T b) {
  // Ensure non-negative inputs
  a = abs(a);
  b = abs(b);
  // Naive loop assuming existence of remainder overload; domains should specialize otherwise.
  while (!is_zero(b)) {
    // Expect ops::remainder to be provided by the domain.
    extern T remainder(T const &, T const &); // declaration only; must be defined by an overload
    T r = remainder(a, b);
    a = b;
    b = r;
  }
  return a;
}

// ----- int overloads -----
// Summary: Concrete implementations of the above for int.

inline bool equals(int a, int b) { return a == b; }
inline bool is_zero(int a) { return a == 0; }
inline bool is_negative(int a) { return a < 0; }
inline int negate(int a) { return -a; }
inline int abs(int a) { return a < 0 ? -a : a; }
/** @brief remainder for int (a % b). */
inline int remainder(int a, int b) { return a % b; }
/** @brief divides for int using remainder. */
inline bool divides(int a, int b) {
  if (a == 0) return b == 0; // by convention, only 0 divides 0
  return b % a == 0;
}
/** @brief gcd for int using Euclidean algorithm. */
inline int gcd(int a, int b) {
  a = abs(a);
  b = abs(b);
  while (b != 0) {
    int r = a % b;
    a = b;
    b = r;
  }
  return a == 0 ? 1 : a;
}

} // namespace ops


