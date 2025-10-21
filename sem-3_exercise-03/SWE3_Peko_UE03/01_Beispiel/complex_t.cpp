#pragma once

#include <iostream>
#include <string>

class complex_t {
public:
  using value_t = double;

  complex_t(value_t real = {}, value_t imaginary = {})
      : m_re(real), m_im(imaginary) {}

  // const after the method name, means const objects can call this method
  constexpr value_t get_real() const {
    return m_re;
  }

  constexpr value_t get_imaginary() const {
    return m_im;
  }

  // Calculations
  // Addition
  complex_t operator+(const complex_t& other) const {
    return complex_t(m_re + other.m_re, m_im + other.m_im);
  }

  complex_t& operator+=(const complex_t& other) {
    *this = *this + other;
    return *this;
  }

  // Subtraction
  complex_t operator-(const complex_t& other) const {
    return complex_t(m_re - other.m_re, m_im - other.m_im);
  }

  complex_t& operator-=(const complex_t& other) {
    *this = *this - other;
    return *this;
  }

  // Multiplication
  complex_t operator*(const complex_t& other) const {
    return complex_t(m_re * other.m_re - m_im * other.m_im,
                     m_re * other.m_im + m_im * other.m_re);
  }

  complex_t& operator*=(const complex_t& other) {
    *this = *this * other;
    return *this;
  }

  // Division
  complex_t operator/(const complex_t& other) const {
    value_t denominator = other.m_re * other.m_re + other.m_im * other.m_im;
    return complex_t((m_re * other.m_re + m_im * other.m_im) / denominator,
                     (m_im * other.m_re - m_re * other.m_im) / denominator);
  }

  complex_t& operator/=(const complex_t& other) {
    *this = *this / other;
    return *this;
  }

  // Unary
  // Minus
  complex_t operator-() const {
    return complex_t(-m_re, -m_im);
  }

  // Comparisons
  // C++17
  bool operator==(const complex_t& other) const {
    return m_re == other.m_re && m_im == other.m_im;
  }
  bool operator!=(const complex_t& other) const {
    return !(*this == other);
  }
  bool operator<(const complex_t& rhs) const {
    return (m_re < rhs.m_re) || (m_re == rhs.m_re && m_im < rhs.m_im);
  }
  bool operator>(const complex_t& rhs) const {
    return rhs < *this;
  }
  bool operator<=(const complex_t& rhs) const {
    return !(*this > rhs);
  }
  bool operator>=(const complex_t& rhs) const {
    return !(*this < rhs);
  }

  // Comparisons
  // C++ 20
  auto operator<=>(const complex_t& rhs) const = default;
  // manual impl if needed
  // auto operator<=>(const complex_t& rhs) const {
  //   if (auto cmp = m_re <=> rhs.m_re; cmp != 0) return cmp;
  //   return m_im <=> rhs.m_im;
  // }

  // Complex conjugate
  complex_t conjugate() const {
    return complex_t(m_re, -m_im);
  }

  // Magnitude (absolute value)
  value_t magnitude() const {
    return std::sqrt(magnitude_squared());
  }

  // Magnitude squared
  value_t magnitude_squared() const {
    return m_re * m_re + m_im * m_im;
  }

  std::string as_string() const {
    std::string result = std::to_string(m_re);
    if (m_im >= 0) {
      result += " + " + std::to_string(m_im) + "i";
    } else {
      result += " - " + std::to_string(-m_im) + "i";
    }
    return result;
  }

  // Stream output
  friend std::ostream& operator<<(std::ostream& os, const complex_t& c) {
    os << c.as_string();
    return os;
  }

  // Stream input
  friend std::istream& operator>>(std::istream& is, complex_t& c) {
    value_t re, im;
    char plus_or_minus, i_char;
    
    // Read real part
    if (!(is >> re)) {
      is.setstate(std::ios::failbit);
      return is;
    }
    
    // Read the sign (+ or -)
    if (!(is >> plus_or_minus) || (plus_or_minus != '+' && plus_or_minus != '-')) {
      is.setstate(std::ios::failbit);
      return is;
    }
    
    // Read imaginary part
    if (!(is >> im)) {
      is.setstate(std::ios::failbit);
      return is;
    }
    
    // Read the 'i' character
    if (!(is >> i_char) || (i_char != 'i' && i_char != 'I')) {
      is.setstate(std::ios::failbit);
      return is;
    }
    
    // Apply the sign to the imaginary part
    if (plus_or_minus == '-') {
      im = -im;
    }
    
    // Set the values only if all input was successful
    c.m_re = re;
    c.m_im = im;
    
    return is;
  }

private:
  value_t m_re{};
  value_t m_im{};
};
