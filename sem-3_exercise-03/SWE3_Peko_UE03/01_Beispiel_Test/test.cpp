#include "pch.h"
#include "../01_Beispiel/rational_t.hpp"
#include <sstream>

using swe3::rational_t;
using swe3::invalid_rational_error;
using swe3::division_by_zero_error;

TEST(Rational_Construct, Defaults) {
	rational_t r;
	EXPECT_EQ(r.get_numerator(), 0);
	EXPECT_EQ(r.get_denominator(), 1);
}

TEST(Rational_Construct, FromInt) {
	rational_t r{5};
	EXPECT_EQ(r.get_numerator(), 5);
	EXPECT_EQ(r.get_denominator(), 1);
}

TEST(Rational_Construct, FromPairNormalizes) {
	rational_t r{2, 4};
	EXPECT_EQ(r.get_numerator(), 1);
	EXPECT_EQ(r.get_denominator(), 2);
	rational_t s{-2, -4};
	EXPECT_EQ(s.get_numerator(), 1);
	EXPECT_EQ(s.get_denominator(), 2);
	rational_t t{-2, 4};
	EXPECT_EQ(t.get_numerator(), -1);
	EXPECT_EQ(t.get_denominator(), 2);
}

TEST(Rational_Construct, ZeroDenThrows) {
	EXPECT_THROW(rational_t(1, 0), invalid_rational_error);
}

TEST(Rational_Predicates, Signs) {
	rational_t a{-1, 2};
	EXPECT_TRUE(a.is_negative());
	EXPECT_FALSE(a.is_positive());
	EXPECT_FALSE(a.is_zero());
	rational_t b{0};
	EXPECT_TRUE(b.is_zero());
	EXPECT_FALSE(b.is_positive());
	EXPECT_FALSE(b.is_negative());
}

TEST(Rational_Strings, AsStringAndStream) {
	rational_t a{3, 1};
	rational_t b{3, 2};
	EXPECT_EQ(a.as_string(), "3");
	EXPECT_EQ(b.as_string(), "3/2");
	std::ostringstream os;
	os << a;
	EXPECT_EQ(os.str(), "<3>");
}

TEST(Rational_Streams, ParseBasicAndErrors) {
	{
		std::istringstream is{"7/8"};
		rational_t r;
		is >> r;
		EXPECT_EQ(r.as_string(), "7/8");
	}
	{
		std::istringstream is{"-5"};
		rational_t r;
		is >> r;
		EXPECT_EQ(r.as_string(), "-5");
	}
	{
		std::istringstream is{"3/0"};
		rational_t r;
		EXPECT_THROW(is >> r, invalid_rational_error);
	}
	{
		std::istringstream is{"abc"};
		rational_t r{1};
		is >> r;
		EXPECT_EQ(r.as_string(), "1");
		EXPECT_TRUE(is.fail());
	}
}

TEST(Rational_Arithmetic, Compound) {
	rational_t a{1, 2};
	rational_t b{1, 3};
	a += b; // 5/6
	EXPECT_EQ(a.as_string(), "5/6");
	a -= rational_t{1, 6}; // 4/6 -> 2/3
	EXPECT_EQ(a.as_string(), "2/3");
	a *= rational_t{3, 5}; // 2/5
	EXPECT_EQ(a.as_string(), "2/5");
	a /= rational_t{2, 1}; // 1/5
	EXPECT_EQ(a.as_string(), "1/5");
}

TEST(Rational_Arithmetic, DivisionByZeroThrows) {
	rational_t a{1, 2};
	EXPECT_THROW(a /= rational_t{0}, division_by_zero_error);
}

TEST(Rational_Arithmetic, BinaryAndIntMix) {
	rational_t r{1, 2};
	EXPECT_EQ(r * -10, rational_t{-5});
	EXPECT_EQ((r * rational_t(20, 2)).as_string(), "5");
	r = 7;
	EXPECT_EQ((r + rational_t(2, 3)).as_string(), "23/3");
	EXPECT_EQ((10 / r / 2 + rational_t(6, 5)).as_string(), "67/35");
}

TEST(Rational_Compare, OrderingAndEq) {
	EXPECT_TRUE(rational_t(1,2) == rational_t(2,4));
	EXPECT_TRUE(rational_t(1,3) < rational_t(1,2));
	EXPECT_TRUE(rational_t(-1,2) < rational_t(0));
	EXPECT_TRUE(rational_t(2) > rational_t(3,2));
}

TEST(Rational_EdgeCases, ZeroNumeratorAlwaysZeroDenOne) {
	rational_t a{0, -5};
	EXPECT_EQ(a.get_numerator(), 0);
	EXPECT_EQ(a.get_denominator(), 1);
	a += rational_t{0, 7};
	EXPECT_EQ(a.get_numerator(), 0);
	EXPECT_EQ(a.get_denominator(), 1);
}

TEST(Rational_EdgeCases, NegativeDenominatorNormalized) {
	rational_t a{1, -3};
	EXPECT_EQ(a.get_numerator(), -1);
	EXPECT_EQ(a.get_denominator(), 3);
}

TEST(Rational_Chains, LongExpressionStaysNormalized) {
	rational_t r{3, 4};
	r = r + rational_t{5, 6} - rational_t{7, 8} + 2;
	EXPECT_EQ(r.as_string(), "49/24");
	r = (r * rational_t{9, 7}) / rational_t{3, 1};
	EXPECT_EQ(r.as_string(), "21/8");
}

TEST(Rational_CopySemantics, CopyAndAssign) {
	rational_t a{4, 6};
	rational_t b{a};
	EXPECT_EQ(b.as_string(), "2/3");
	rational_t c;
	c = a;
	EXPECT_EQ(c.as_string(), "2/3");
}

TEST(Rational_Assignment, SelfAssignmentNoChange) {
	rational_t a{5, 10};
	a = a;
	EXPECT_EQ(a.as_string(), "1/2");
}

TEST(Rational_Compare2, NeqAndLeGe) {
	EXPECT_TRUE(rational_t(1,2) != rational_t(2,3));
	EXPECT_TRUE(rational_t(1,2) <= rational_t(2,3));
	EXPECT_TRUE(rational_t(3,3) >= rational_t(1,1));
}