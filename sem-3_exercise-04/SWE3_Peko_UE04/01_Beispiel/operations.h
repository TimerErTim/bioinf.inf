#pragma once

#include <type_traits>
#include <limits>

// Generic operations abstraction for minimally constraining T used in rational_t<T>.
// Provide overloads for int, and allow other domains to add overloads as needed.

namespace ops {

// equals
template <typename T>
inline bool equals(T const &a, T const &b) {
  return a == b;
}

// is_zero
template <typename T>
inline bool is_zero(T const &a) {
  return equals(a, T{0});
}

// is_negative
template <typename T>
inline bool is_negative(T const &a) {
  return a < T{0};
}

// negate
template <typename T>
inline T negate(T const &a) {
  return T{0} - a;
}

// abs (generic, relies on comparisons and negate)
template <typename T>
inline T abs(T const &a) {
  return is_negative(a) ? negate(a) : a;
}

// remainder (generic fallback disabled)
// Provide for int; other domains can overload.

// divides (generic, uses remainder)
template <typename T>
inline bool divides(T const &a, T const &b) {
  // In absence of a meaningful remainder, conservatively say false unless a is 1 or -1.
  // Domains should provide a stronger overload when available.
  return equals(abs(a), T{1});
}

// gcd (generic Euclidean algorithm; relies on remainder). Domains without remainder
// should overload this to return a multiplicative identity (e.g., one()) if appropriate.
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

inline bool equals(int a, int b) { return a == b; }
inline bool is_zero(int a) { return a == 0; }
inline bool is_negative(int a) { return a < 0; }
inline int negate(int a) { return -a; }
inline int abs(int a) { return a < 0 ? -a : a; }
inline int remainder(int a, int b) { return a % b; }
inline bool divides(int a, int b) {
  if (a == 0) return b == 0; // by convention, only 0 divides 0
  return b % a == 0;
}
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


