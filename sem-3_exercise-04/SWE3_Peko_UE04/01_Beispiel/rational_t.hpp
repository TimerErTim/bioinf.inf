#pragma once

#include <concepts>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

#include "errors.hpp"

template <typename T>
// Attempting recursive reference
concept RationalElement =
    std::regular<T> &&         // copyable, assignable, equality comparable
    std::totally_ordered<T> && // <, <=, >, >=, ==, !=
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

      // stream I/O
      { os << a } -> std::same_as<std::ostream&>;
      { is >> a } -> std::same_as<std::istream&>;
    };

template <RationalElement T>
class rational_t {
public:
  using value_type = T;

  rational_t() noexcept : numerator_(T{0}), denominator_(T{1}) {}
  rational_t(const value_type &numerator) noexcept : numerator_(numerator), denominator_(T{1}) {}
  rational_t(const value_type &numerator, const value_type &denominator) : numerator_(numerator), denominator_(denominator) {
    if (denominator == T{0}) {
      throw invalid_rational_error("denominator must not be zero");
    }
    normalize();
  }
  // Default implementation of rule of five is enough

  value_type const &get_numerator() const noexcept { return numerator_; }
  value_type const &get_denominator() const noexcept { return denominator_; }

  bool is_negative() const noexcept { return numerator_ < T{0}; }
  bool is_positive() const noexcept { return numerator_ > T{0}; }
  bool is_zero() const noexcept { return numerator_ == T{0}; }

  // Replace this rational with its multiplicative inverse (swap
  // numerator/denominator)
  void inverse() {
    if (is_zero()) {  // numerator is zero [would -> denominator is zero!]
      throw division_by_zero_error("cannot invert zero");
    }
    std::swap(numerator_, denominator_);
    normalize();
  }

  // Write rational as <[numerator]/[denominator]> or <[numerator]> if simplifiable
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
  rational_t &operator+=(const rational_t &rhs) noexcept {
    numerator_ = numerator_ * rhs.denominator_ + rhs.numerator_ * denominator_;
    denominator_ = denominator_ * rhs.denominator_;
    normalize();
    return *this;
  }
  rational_t &operator-=(const rational_t &rhs) noexcept {
    numerator_ = numerator_ * rhs.denominator_ - rhs.numerator_ * denominator_;
    denominator_ = denominator_ * rhs.denominator_;
    normalize();
    return *this;
  }
  rational_t &operator*=(const rational_t &rhs) noexcept {
    numerator_ = numerator_ * rhs.numerator_;
    denominator_ = denominator_ * rhs.denominator_;
    normalize();
    return *this;
  }
  rational_t &operator/=(const rational_t &rhs) {
    if (rhs.is_zero()) {
      throw division_by_zero_error("division by zero rational number");
    }
    numerator_ = numerator_ * rhs.denominator_;
    denominator_ = denominator_ * rhs.numerator_;
    normalize();
    return *this;
  }

  // Binary arithmetic operators (delegating to compound), defined as friends
  friend rational_t operator+(rational_t lhs, const rational_t &rhs) noexcept {
    lhs += rhs;
    return lhs;
  }
  friend rational_t operator-(rational_t lhs, const rational_t &rhs) noexcept {
    lhs -= rhs;
    return lhs;
  }
  friend rational_t operator*(rational_t lhs, const rational_t &rhs) noexcept {
    lhs *= rhs;
    return lhs;
  }
  friend rational_t operator/(rational_t lhs, const rational_t &rhs) {
    lhs /= rhs;
    return lhs;
  }

  // Comparisons
  friend bool operator==(const rational_t &a, const rational_t &b) noexcept {
    return ops::equals(a.numerator_, b.numerator_) &&
           ops::equals(a.denominator_, b.denominator_);
  }
  friend bool operator!=(const rational_t &a, const rational_t &b) noexcept {
    return !(a == b);
  }
  friend bool operator<(const rational_t &a, const rational_t &b) noexcept {
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
  friend std::ostream &operator<<(std::ostream &os, const rational_t &r) {
    os << r.as_string();
    return os;
  }
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
    if (ops::is_zero(den)) {
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

    // Reduce when a gcd is defined for T, a > 0 guarandeed
    T a = is_positive() ? numerator_ : -numerator_;
    T b = denominator_;
    while (b > T{0}) {
        T c = a;
        a = b % c;
        b = c;
    }
    numerator_ = numerator_ / a;
    denominator_ = denominator_ / a;
  }

  value_type numerator_;
  value_type denominator_;
};

// Convenience overloads for mixing with int on the LHS for rational_t<int>
inline rational_t<int> operator+(int lhs, const rational_t<int> &rhs) noexcept {
  return rational_t<int>(lhs) + rhs;
}
inline rational_t<int> operator-(int lhs, const rational_t<int> &rhs) noexcept {
  return rational_t<int>(lhs) - rhs;
}
inline rational_t<int> operator*(int lhs, const rational_t<int> &rhs) noexcept {
  return rational_t<int>(lhs) * rhs;
}
inline rational_t<int> operator/(int lhs, const rational_t<int> &rhs) {
  return rational_t<int>(lhs) / rhs;
}
