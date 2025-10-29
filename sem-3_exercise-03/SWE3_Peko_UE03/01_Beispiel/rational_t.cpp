//
// Implementation of swe3::rational_t
//
// Representation and invariants
// - The denominator is never 0 and is always kept positive.
// - Zero is represented canonically as 0/1.
// - All instances are kept in reduced (canonical) form via normalize().
//
// Exceptions
// - Constructing with denominator 0 throws invalid_rational_error (logic error in API use).
// - Dividing by a rational zero throws division_by_zero_error (runtime error at operation time).
//
// Algorithms
// - normalize(): ensures positive denominator, canonical zero, and reduction by gcd.
// - gcd_int(): iterative Euclidean algorithm, O(log min(|a|,|b|)).
// - Comparisons: cross-multiplication on 64-bit intermediates to avoid rounding error.
// - Conversions: implicit int -> rational_t is allowed for ergonomic mixed arithmetic.
//
#include "rational_t.hpp"

#include <sstream>
#include <ostream>
#include <istream>
#include <cstdlib>

namespace swe3 {

rational_t::rational_t() noexcept : numerator_(0), denominator_(1) {}

rational_t::rational_t(value_type numerator) noexcept : numerator_(numerator), denominator_(1) {}

rational_t::rational_t(value_type numerator, value_type denominator): numerator_(numerator), denominator_(denominator) {
	// Guard against invalid input at construction time. This is an API misuse
	// (logic error), so we report via invalid_rational_error.
	if (!is_consistent()) {
		throw invalid_rational_error("denominator must not be zero");
	}
	normalize();
}

rational_t::rational_t(const rational_t&) noexcept = default;

rational_t& rational_t::operator=(const rational_t& other) noexcept {
	// Support self-assignment and plain member-wise copy since invariants are
	// already satisfied on both sides; no need to re-normalize.
	if (this == &other) return *this;
	numerator_ = other.numerator_;
	denominator_ = other.denominator_;
	return *this;
}

rational_t::value_type rational_t::get_numerator() const noexcept { return numerator_; }
rational_t::value_type rational_t::get_denominator() const noexcept { return denominator_; }

bool rational_t::is_negative() const noexcept { return numerator_ < 0; }
bool rational_t::is_positive() const noexcept { return numerator_ > 0; }
bool rational_t::is_zero() const noexcept { return numerator_ == 0; }

std::string rational_t::as_string() const {
	std::ostringstream out;
	if (denominator_ == 1) {
		out << numerator_;
	} else {
		out << numerator_ << '/' << denominator_;
	}
	return out.str();
}

rational_t& rational_t::operator+=(const rational_t& rhs) noexcept {
	// a/b + c/d := (ad + bc) / (bd). Use 64-bit temporaries to reduce the
	// risk of intermediate overflow before we normalize back to int.
	const long long new_num = static_cast<long long>(numerator_) * rhs.denominator_
		+ static_cast<long long>(rhs.numerator_) * denominator_;
	const long long new_den = static_cast<long long>(denominator_) * rhs.denominator_;
	numerator_ = static_cast<value_type>(new_num);
	denominator_ = static_cast<value_type>(new_den);
	normalize();
	return *this;
}

rational_t& rational_t::operator-=(const rational_t& rhs) noexcept {
	// a/b - c/d := (ad - bc) / (bd). Same 64-bit precaution as in operator+=.
	const long long new_num = static_cast<long long>(numerator_) * rhs.denominator_
		- static_cast<long long>(rhs.numerator_) * denominator_;
	const long long new_den = static_cast<long long>(denominator_) * rhs.denominator_;
	numerator_ = static_cast<value_type>(new_num);
	denominator_ = static_cast<value_type>(new_den);
	normalize();
	return *this;
}

rational_t& rational_t::operator*=(const rational_t& rhs) noexcept {
	// (a/b) * (c/d) := (ac) / (bd). 64-bit temporaries again for safety.
	const long long new_num = static_cast<long long>(numerator_) * rhs.numerator_;
	const long long new_den = static_cast<long long>(denominator_) * rhs.denominator_;
	numerator_ = static_cast<value_type>(new_num);
	denominator_ = static_cast<value_type>(new_den);
	normalize();
	return *this;
}

rational_t& rational_t::operator/=(const rational_t& rhs) {
	// Division by a rational zero is a runtime domain error.
	if (rhs.is_zero()) {
		throw division_by_zero_error("division by zero rational number");
	}
	const long long new_num = static_cast<long long>(numerator_) * rhs.denominator_;
	const long long new_den = static_cast<long long>(denominator_) * rhs.numerator_;
	numerator_ = static_cast<value_type>(new_num);
	denominator_ = static_cast<value_type>(new_den);
	normalize();
	return *this;
}

bool operator==(const rational_t& a, const rational_t& b) noexcept {
	// Canonical form ensures structural equality matches arithmetic equality.
	return a.numerator_ == b.numerator_ && a.denominator_ == b.denominator_;
}

bool operator!=(const rational_t& a, const rational_t& b) noexcept { return !(a == b); }

bool operator<(const rational_t& a, const rational_t& b) noexcept {
	// Compare without converting to floating-point: a/b < c/d <=> ad < cb.
	// 64-bit intermediates reduce overflow risk compared to 32-bit.
	return static_cast<long long>(a.numerator_) * b.denominator_
		< static_cast<long long>(b.numerator_) * a.denominator_;
}

bool operator>(const rational_t& a, const rational_t& b) noexcept { return b < a; }
bool operator<=(const rational_t& a, const rational_t& b) noexcept { return !(b < a); }
bool operator>=(const rational_t& a, const rational_t& b) noexcept { return !(a < b); }

rational_t operator+(rational_t lhs, const rational_t& rhs) noexcept { lhs += rhs; return lhs; }
rational_t operator-(rational_t lhs, const rational_t& rhs) noexcept { lhs -= rhs; return lhs; }
rational_t operator*(rational_t lhs, const rational_t& rhs) noexcept { lhs *= rhs; return lhs; }
rational_t operator/(rational_t lhs, const rational_t& rhs) { lhs /= rhs; return lhs; }

std::ostream& operator<<(std::ostream& os, const rational_t& r) {
	// Output in the assignment's required angle-bracket format.
	os << '<' << r.as_string() << '>';
	return os;
}

std::istream& operator>>(std::istream& is, rational_t& r) {
	// Accepted formats:
	//   n      -> integer becomes n/1
	//   n/d    -> proper or improper fraction
	// On parse failure of the first integer, leave r unchanged and propagate stream state.
	// A denominator of 0 sets failbit and throws invalid_rational_error.
	rational_t::value_type num = 0;
	rational_t::value_type den = 1;
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
	if (den == 0) {
		is.setstate(std::ios::failbit);
		throw invalid_rational_error("attempt to construct rational with zero denominator from stream");
	}
	r = rational_t(num, den);
	return is;
}

int rational_t::gcd_int(int a, int b) noexcept {
	// Euclidean algorithm for gcd on signed integers.
	// We use absolute values to support negative inputs.
	// If both inputs are 0 (not expected in normal flow), return 1 so that
	// normalization stays well-defined; 0 is canonicalized earlier to 0/1.
	a = std::abs(a);
	b = std::abs(b);
	while (b != 0) {
		const int t = a % b;
		a = b;
		b = t;
	}
	return a == 0 ? 1 : a;
}

bool rational_t::is_consistent() const noexcept {
	return denominator_ != 0;
}

void rational_t::normalize() noexcept {
	// Force denominator to be positive so sign is carried by numerator only.
	if (denominator_ < 0) {
		numerator_ = -numerator_;
		denominator_ = -denominator_;
	}
	// Canonical zero: 0/anything -> 0/1
	if (numerator_ == 0) {
		denominator_ = 1;
		return;
	}
	// Reduce fraction to lowest terms.
	const int g = gcd_int(numerator_, denominator_);
	numerator_ /= g;
	denominator_ /= g;
}

rational_t operator+(int lhs, const rational_t& rhs) noexcept { return rational_t(lhs) + rhs; }
rational_t operator-(int lhs, const rational_t& rhs) noexcept { return rational_t(lhs) - rhs; }
rational_t operator*(int lhs, const rational_t& rhs) noexcept { return rational_t(lhs) * rhs; }
rational_t operator/(int lhs, const rational_t& rhs) { return rational_t(lhs) / rhs; }

} // namespace swe3


