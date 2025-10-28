#include "pch.h"
#include "rational_t.hpp"
#include <sstream>

using swe3::rational_t;
using swe3::invalid_rational_error;
using swe3::division_by_zero_error;

TEST(Rational_Construct, Defaults) {
	// Arrange & Act
	rational_t r;
	// Assert
	EXPECT_EQ(r.get_numerator(), 0);
	EXPECT_EQ(r.get_denominator(), 1);
}

TEST(Rational_Construct, FromInt) {
	// Arrange & Act
	rational_t r{5};
	// Assert
	EXPECT_EQ(r.get_numerator(), 5);
	EXPECT_EQ(r.get_denominator(), 1);
}

TEST(Rational_Construct, FromPairNormalizes) {
	{
		// Arrange & Act
		rational_t r{2, 4};
		// Assert
		EXPECT_EQ(r.get_numerator(), 1);
		EXPECT_EQ(r.get_denominator(), 2);
	}
	{
		// Arrange & Act
		rational_t s{-2, -4};
		// Assert
		EXPECT_EQ(s.get_numerator(), 1);
		EXPECT_EQ(s.get_denominator(), 2);
	}
	{
		// Arrange & Act
		rational_t t{-2, 4};
		// Assert
		EXPECT_EQ(t.get_numerator(), -1);
		EXPECT_EQ(t.get_denominator(), 2);
	}
}

TEST(Rational_Construct, ZeroDenThrows) {
	// Arrange
	auto act = [] { rational_t x{1, 0}; };
	// Assert
	EXPECT_THROW(act(), invalid_rational_error);
}

TEST(Rational_Predicates, Signs) {
	{
		// Arrange & Act
		rational_t a{-1, 2};
		// Assert
		EXPECT_TRUE(a.is_negative());
		EXPECT_FALSE(a.is_positive());
		EXPECT_FALSE(a.is_zero());
	}
	{
		// Arrange & Act
		rational_t b{0};
		// Assert
		EXPECT_TRUE(b.is_zero());
		EXPECT_FALSE(b.is_positive());
		EXPECT_FALSE(b.is_negative());
	}
}

TEST(Rational_Strings, AsStringAndStream) {
	// Arrange
	rational_t a{3, 1};
	rational_t b{3, 2};
	// Act
	const auto a_str = a.as_string();
	const auto b_str = b.as_string();
	// Assert
	EXPECT_EQ(a_str, "3");
	EXPECT_EQ(b_str, "3/2");
	// Arrange
	std::ostringstream os;
	// Act
	os << a;
	// Assert
	EXPECT_EQ(os.str(), "<3>");
}

TEST(Rational_Streams, ParseBasicAndErrors) {
	{
		// Arrange
		std::istringstream is{"7/8"};
		rational_t r;
		// Act
		is >> r;
		const auto s = r.as_string();
		// Assert
		EXPECT_EQ(s, "7/8");
	}
	{
		// Arrange
		std::istringstream is{"-5"};
		rational_t r;
		// Act
		is >> r;
		const auto s = r.as_string();
		// Assert
		EXPECT_EQ(s, "-5");
	}
	{
		// Arrange
		std::istringstream is{"3/0"};
		rational_t r;
		// Act
		auto act = [&] { is >> r; };
		// Assert
		EXPECT_THROW(act(), invalid_rational_error);
	}
	{
		// Arrange
		std::istringstream is{"abc"};
		rational_t r{1};
		// Act
		is >> r;
		const auto s = r.as_string();
		const bool failed = is.fail();
		// Assert
		EXPECT_EQ(s, "1");
		EXPECT_TRUE(failed);
	}
}

TEST(Rational_Arithmetic, Compound) {
	// Arrange
	rational_t a{1, 2};
	rational_t b{1, 3};
	// Act
	a += b; // 5/6
	// Assert
	EXPECT_EQ(a.as_string(), "5/6");
	// Act
	a -= rational_t{1, 6}; // 4/6 -> 2/3
	// Assert
	EXPECT_EQ(a.as_string(), "2/3");
	// Act
	a *= rational_t{3, 5}; // 2/5
	// Assert
	EXPECT_EQ(a.as_string(), "2/5");
	// Act
	a /= rational_t{2, 1}; // 1/5
	// Assert
	EXPECT_EQ(a.as_string(), "1/5");
}

TEST(Rational_Arithmetic, DivisionByZeroThrows) {
	// Arrange
	rational_t a{1, 2};
	// Act & Assert
	EXPECT_THROW(a /= rational_t{0}, division_by_zero_error);
}

TEST(Rational_Arithmetic, BinaryAndIntMix) {
	// Arrange
	rational_t r{1, 2};
	// Act
	const auto prod_neg = r * -10;
	const auto prod = r * rational_t(20, 2);
	r = 7;
	const auto sum = r + rational_t(2, 3);
	const auto expr = 10 / r / 2 + rational_t(6, 5);
	// Assert
	EXPECT_EQ(prod_neg, rational_t{-5});
	EXPECT_EQ(prod.as_string(), "5");
	EXPECT_EQ(sum.as_string(), "23/3");
	EXPECT_EQ(expr.as_string(), "67/35");
}

TEST(Rational_Compare, OrderingAndEq) {
	// Arrange
	const rational_t a{1,2};
	const rational_t b{2,4};
	const rational_t c{1,3};
	const rational_t d{1,2};
	const rational_t e{-1,2};
	const rational_t f{0};
	const rational_t g{2};
	const rational_t h{3,2};
	// Act
	const bool eq_ab = (a == b);
	const bool lt_cd = (c < d);
	const bool lt_ef = (e < f);
	const bool gt_gh = (g > h);
	// Assert
	EXPECT_TRUE(eq_ab);
	EXPECT_TRUE(lt_cd);
	EXPECT_TRUE(lt_ef);
	EXPECT_TRUE(gt_gh);
}

TEST(Rational_EdgeCases, ZeroNumeratorAlwaysZeroDenOne) {
	// Arrange & Act
	rational_t a{0, -5};
	// Assert
	EXPECT_EQ(a.get_numerator(), 0);
	EXPECT_EQ(a.get_denominator(), 1);
	// Act
	a += rational_t{0, 7};
	// Assert
	EXPECT_EQ(a.get_numerator(), 0);
	EXPECT_EQ(a.get_denominator(), 1);
}

TEST(Rational_EdgeCases, NegativeDenominatorNormalized) {
	// Arrange & Act
	rational_t a{1, -3};
	// Assert
	EXPECT_EQ(a.get_numerator(), -1);
	EXPECT_EQ(a.get_denominator(), 3);
}

TEST(Rational_Chains, LongExpressionStaysNormalized) {
	// Arrange
	rational_t r{3, 4};
	// Act
	r = r + rational_t{5, 6} - rational_t{7, 8} + 2;
	// Assert
	EXPECT_EQ(r.as_string(), "65/24");
	// Act
	r = (r * rational_t{9, 7}) / rational_t{3, 1};
	// Assert
	EXPECT_EQ(r.as_string(), "65/56");
}

TEST(Rational_CopySemantics, CopyAndAssign) {
	// Arrange
	rational_t a{4, 6};
	// Act
	rational_t b{a};
	// Assert
	EXPECT_EQ(b.as_string(), "2/3");
	// Arrange
	rational_t c;
	// Act
	c = a;
	// Assert
	EXPECT_EQ(c.as_string(), "2/3");
}

TEST(Rational_Assignment, SelfAssignmentNoChange) {
	// Arrange
	rational_t a{5, 10};
	// Act
	a = a;
	// Assert
	EXPECT_EQ(a.as_string(), "1/2");
}

TEST(Rational_Compare, NeqAndLeGe) {
	// Arrange
	const rational_t a{1,2};
	const rational_t b{2,3};
	const rational_t c{3,3};
	const rational_t d{1,1};
	// Act
	const bool ne_ab = (a != b);
	const bool le_ab = (a <= b);
	const bool ge_cd = (c >= d);
	// Assert
	EXPECT_TRUE(ne_ab);
	EXPECT_TRUE(le_ab);
	EXPECT_TRUE(ge_cd);
}