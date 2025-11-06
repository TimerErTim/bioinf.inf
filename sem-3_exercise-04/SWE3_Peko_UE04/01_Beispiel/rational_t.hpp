#pragma once

#include <concepts>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

#include "errors.hpp"

// -----------------------------------------------------------------------------
// Summary
// Generic rational number type parameterized by T. Maintains the invariant
// denominator >= 0 and canonical zero 0/1. Normalization uses Euclidean gcd
// (requires %), comparisons use cross-multiplication to avoid dependence on
// reduced form.
// -----------------------------------------------------------------------------

/**
 * Concept constraining element type T used by rational_t<T>.
 * Requires arithmetic (+,-,*,/,%), unary minus, comparisons, and streaming.
 */
template <typename T>
// Attempting recursive reference
concept RationalElement =
    std::regular<T> &&         // copyable, assignable, equality comparable
    std::constructible_from<T, int> && // must be constructible from int
                                       // literals like 0, 1
    requires(T a, T b, std::ostream os, std::istream is) {
      // arithmetic
      { a + b } -> std::convertible_to<T>;
      { a - b } -> std::convertible_to<T>;
      { a * b } -> std::convertible_to<T>;
      { a / b } -> std::convertible_to<T>;
      { a % b } -> std::convertible_to<T>;

      // unary minus
      { -a } -> std::convertible_to<T>;

      // comparisons
      { a == b } -> std::same_as<bool>;
      { a != b } -> std::same_as<bool>;
      { a < b } -> std::same_as<bool>;

      // stream I/O
      { os << a } -> std::same_as<std::ostream&>;
      { is >> a } -> std::same_as<std::istream&>;
    };

template <RationalElement T>
class rational_t {
public:
  using value_type = T;

  /**
   * @brief Construct 0/1.
   */
  rational_t() noexcept : numerator_(T{0}), denominator_(T{1}) {}
  /**
   * @brief Construct numerator/1.
   * @param numerator Numerator value.
   */
  rational_t(const value_type &numerator) noexcept : numerator_(numerator), denominator_(T{1}) {}
  /**
   * @brief Construct numerator/denominator.
   * @param numerator Numerator value.
   * @param denominator Denominator value, must not be zero.
   * @throws invalid_rational_error if denominator == 0
   */
  rational_t(const value_type &numerator, const value_type &denominator) : numerator_(numerator), denominator_(denominator) {
    if (denominator_ == T{0}) {
      throw invalid_rational_error("denominator must not be zero");
    }
    normalize();
  }
  // Default implementation of rule of five is enough

  /**
   * @brief Access numerator.
   * @return const reference to numerator
   */
  value_type const &get_numerator() const noexcept { return numerator_; }
  /**
   * @brief Access denominator.
   * @return const reference to denominator
   */
  value_type const &get_denominator() const noexcept { return denominator_; }

  /** @brief True if numerator < 0. */
  bool is_negative() const noexcept { return numerator_ < T{0}; }
  /** @brief True if numerator > 0. */
  bool is_positive() const noexcept { return T{0} < numerator_; }
  /** @brief True if numerator == 0. */
  bool is_zero() const noexcept { return numerator_ == T{0}; }

  // Replace this rational with its multiplicative inverse. Swapping numerator
  // and denominator preserves the invariant via normalize().
  /**
   * @brief In-place multiplicative inverse.
   * @throws division_by_zero_error if numerator == 0
   */
  void inverse() {
    if (is_zero()) {  // numerator is zero [would -> denominator is zero!]
      throw division_by_zero_error("cannot invert zero");
    }
    std::swap(numerator_, denominator_);
    normalize();
  }

  // Write rational as <[numerator]/[denominator]> or <[numerator]> if simplifiable
  /**
   * @brief Format as "<n/d>" or "<n>" when d == 1.
   */
  std::string as_string() const {
    std::ostringstream out;
    out << "<";
    if (denominator_ == T{1}) {
      out << numerator_;
    } else {
      out << numerator_ << "/" << denominator_;
    }
    out << ">";
    return out.str();
  }

  // Compound assignment operators
  /** @brief Add and assign. */
  rational_t &operator+=(const rational_t &rhs) noexcept {
    // Principle: Add rationals via common denominator.
    // (a/b) + (c/d) = (ad + cb) / bd
    numerator_ = numerator_ * rhs.denominator_ + rhs.numerator_ * denominator_;
    denominator_ = denominator_ * rhs.denominator_;
    normalize();
    return *this;
  }
  /** @brief Subtract and assign. */
  rational_t &operator-=(const rational_t &rhs) noexcept {
    // Principle: Subtract via common denominator.
    // (a/b) - (c/d) = (ad - cb) / bd
    numerator_ = numerator_ * rhs.denominator_ - rhs.numerator_ * denominator_;
    denominator_ = denominator_ * rhs.denominator_;
    normalize();
    return *this;
  }
  /** @brief Multiply and assign. */
  rational_t &operator*=(const rational_t &rhs) noexcept {
    // Principle: Multiply numerators and denominators directly.
    // (a/b) * (c/d) = (ac) / (bd)
    numerator_ = numerator_ * rhs.numerator_;
    denominator_ = denominator_ * rhs.denominator_;
    normalize();
    return *this;
  }
  /** @brief Divide and assign.
   *  @throws division_by_zero_error if rhs == 0
   */
  rational_t &operator/=(const rational_t &rhs) {
    if (rhs.is_zero()) {
      throw division_by_zero_error("division by zero rational number");
    }
    // Principle: Division is multiplication by the multiplicative inverse.
    // (a/b) / (c/d) = (a/b) * (d/c)
    auto inv = rhs;
    inv.inverse();
    *this *= inv;
    return *this;
  }

  // Binary arithmetic operators (delegating to compound), defined as friends
  /** @brief a + b */
  friend rational_t operator+(rational_t lhs, const rational_t &rhs) noexcept {
    lhs += rhs;
    return lhs;
  }
  /** @brief a - b */
  friend rational_t operator-(rational_t lhs, const rational_t &rhs) noexcept {
    lhs -= rhs;
    return lhs;
  }
  /** @brief a * b */
  friend rational_t operator*(rational_t lhs, const rational_t &rhs) noexcept {
    lhs *= rhs;
    return lhs;
  }
  /** @brief a / b (throws if b==0) */
  friend rational_t operator/(rational_t lhs, const rational_t &rhs) {
    lhs /= rhs;
    return lhs;
  }

  // Comparisons
  friend bool operator==(const rational_t &a, const rational_t &b) noexcept {
    // Principle: Cross-multiplication avoids reliance on prior reduction.
    // a/b == c/d  <=>  a*d == c*b (denominators non-zero, ensured by invariant)
    return (a.numerator_ * b.denominator_) == (b.numerator_ * a.denominator_);
  }
  friend bool operator!=(const rational_t &a, const rational_t &b) noexcept {
    return !(a == b);
  }
  friend bool operator<(const rational_t &a, const rational_t &b) noexcept {
    // Principle: Compare cross-products to avoid division/rounding.
    return (a.numerator_ * b.denominator_) < (b.numerator_ * a.denominator_);
  }
  friend bool operator>(const rational_t &a, const rational_t &b) noexcept {
    return b < a;
  }
  friend bool operator<=(const rational_t &a, const rational_t &b) noexcept {
    return !(b < a);
  }
  friend bool operator>=(const rational_t &a, const rational_t &b) noexcept {
    return !(a < b);
  }

  // Stream operators
  /** @brief Write as_string() to stream. */
  friend std::ostream &operator<<(std::ostream &os, const rational_t &r) {
    os << r.as_string();
    return os;
  }
  /**
   * @brief Parse formats "<n>" or "<n/d>".
   * @throws invalid_rational_error if parsed denominator == 0
   */
  friend std::istream &operator>>(std::istream &is, rational_t &r) {
    // Accepted formats: "<n>" or "<n/d>"
    T num{};
    T den{1};
    
    is >> std::ws;
    
    // Expect opening angle bracket
    if (is.peek() != '<') {
      is.setstate(std::ios::failbit);
      return is;
    }
    is.get();
    
    // Read numerator
    if (!(is >> num)) {
      is.setstate(std::ios::failbit);
      return is;
    }
    
    // Check for optional denominator
    if (is.peek() == '/') {
      is.get();
      if (!(is >> den)) {
        is.setstate(std::ios::failbit);
        return is;
      }
    }
    
    // Expect closing angle bracket
    is >> std::ws;
    if (is.peek() != '>') {
      is.setstate(std::ios::failbit);
      return is;
    }
    is.get();
    
    // Validate denominator
    if (den == T{0}) {
      is.setstate(std::ios::failbit);
      throw invalid_rational_error(
          "attempt to construct rational with zero denominator from stream");
    }
    
    r = rational_t(num, den);
    return is;
  }

private:
  // Keep invariant: denominator non-negative, zero normalized, and reduce if
  // possible
  void normalize() noexcept {
    // Move sign to numerator if denominator is negative
    if (denominator_ < T{0}) {
      numerator_ = -numerator_;
      denominator_ = -denominator_;
    }
    // Canonical zero: 0/x -> 0/1
    if (is_zero()) {
      denominator_ = T{1};
      return;
    }

    // Principle: Euclidean reduction by gcd (requires %).
    // While (b>0) { c=b; b=a%b; a=c; } then divide numerator and denominator by a.
    // a > 0 guaranteed by taking absolute value via branch.
    T a = is_positive() ? numerator_ : -numerator_;
    T b = denominator_;
    while (T{0} < b) {
        T c = b;
        b = a % b;
        a = c;
    }
    numerator_ = numerator_ / a;
    denominator_ = denominator_ / a;
  }

  value_type numerator_;
  value_type denominator_;
};

// Convenience overloads for mixing with int on the LHS for rational_t<int>
/** @brief int + rational */
inline rational_t<int> operator+(int lhs, const rational_t<int> &rhs) noexcept {
  return rational_t<int>(lhs) + rhs;
}
/** @brief int - rational */
inline rational_t<int> operator-(int lhs, const rational_t<int> &rhs) noexcept {
  return rational_t<int>(lhs) - rhs;
}
/** @brief int * rational */
inline rational_t<int> operator*(int lhs, const rational_t<int> &rhs) noexcept {
  return rational_t<int>(lhs) * rhs;
}
/** @brief int / rational */
inline rational_t<int> operator/(int lhs, const rational_t<int> &rhs) {
  return rational_t<int>(lhs) / rhs;
}
