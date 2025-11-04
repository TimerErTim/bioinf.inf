#include "pch.h"
#include "rational_t.hpp"
#include "matrix_t.hpp"
#include <sstream>

using R = rational_t<int>;

TEST(Rational_Construct, Defaults) {
	// Arrange & Act
	R r;
	// Assert
	EXPECT_EQ(r.get_numerator(), 0);
	EXPECT_EQ(r.get_denominator(), 1);
}

TEST(Rational_Construct, FromInt) {
	// Arrange & Act
	R r{5};
	// Assert
	EXPECT_EQ(r.get_numerator(), 5);
	EXPECT_EQ(r.get_denominator(), 1);
}

TEST(Rational_Construct, FromPairNormalizes) {
	{
		// Arrange & Act
		R r{2, 4};
		// Assert
		EXPECT_EQ(r.get_numerator(), 1);
		EXPECT_EQ(r.get_denominator(), 2);
	}
	{
		// Arrange & Act
		R s{-2, -4};
		// Assert
		EXPECT_EQ(s.get_numerator(), 1);
		EXPECT_EQ(s.get_denominator(), 2);
	}
	{
		// Arrange & Act
		R t{-2, 4};
		// Assert
		EXPECT_EQ(t.get_numerator(), -1);
		EXPECT_EQ(t.get_denominator(), 2);
	}
}

TEST(Rational_Construct, ZeroDenThrows) {
	// Arrange
	auto act = [] { R x{1, 0}; };
	// Assert
	EXPECT_THROW(act(), ::invalid_rational_error);
}

TEST(Rational_Predicates, Signs) {
	{
		// Arrange & Act
		R a{-1, 2};
		// Assert
		EXPECT_TRUE(a.is_negative());
		EXPECT_FALSE(a.is_positive());
		EXPECT_FALSE(a.is_zero());
	}
	{
		// Arrange & Act
		R b{0};
		// Assert
		EXPECT_TRUE(b.is_zero());
		EXPECT_FALSE(b.is_positive());
		EXPECT_FALSE(b.is_negative());
	}
}

TEST(Rational_Strings, AsStringAndStream) {
	// Arrange
	R a{3, 1};
	R b{3, 2};
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
		R r;
		// Act
		is >> r;
		const auto s = r.as_string();
		// Assert
		EXPECT_EQ(s, "7/8");
	}
	{
		// Arrange
		std::istringstream is{"-5"};
		R r;
		// Act
		is >> r;
		const auto s = r.as_string();
		// Assert
		EXPECT_EQ(s, "-5");
	}
	{
		// Arrange
		std::istringstream is{"3/0"};
		R r;
		// Act
		auto act = [&] { is >> r; };
		// Assert
		EXPECT_THROW(act(), invalid_rational_error);
	}
	{
		// Arrange
		std::istringstream is{"abc"};
		R r{1};
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
	R a{1, 2};
	R b{1, 3};
	// Act
	a += b; // 5/6
	// Assert
	EXPECT_EQ(a.as_string(), "5/6");
	// Act
	a -= R{1, 6}; // 4/6 -> 2/3
	// Assert
	EXPECT_EQ(a.as_string(), "2/3");
	// Act
	a *= R{3, 5}; // 2/5
	// Assert
	EXPECT_EQ(a.as_string(), "2/5");
	// Act
	a /= R{2, 1}; // 1/5
	// Assert
	EXPECT_EQ(a.as_string(), "1/5");
}

TEST(Rational_Arithmetic, DivisionByZeroThrows) {
	// Arrange
	R a{1, 2};
	// Act & Assert
	EXPECT_THROW(a /= R{0}, division_by_zero_error);
}

TEST(Rational_Arithmetic, BinaryAndIntMix) {
	// Arrange
	R r{1, 2};
	// Act
	const auto prod_neg = r * -10;
	const auto prod = r * R(20, 2);
	r = 7;
	const auto sum = r + R(2, 3);
	const auto expr = 10 / r / 2 + R(6, 5);
	// Assert
	EXPECT_EQ(prod_neg, R{-5});
	EXPECT_EQ(prod.as_string(), "5");
	EXPECT_EQ(sum.as_string(), "23/3");
	EXPECT_EQ(expr.as_string(), "67/35");
}

TEST(Rational_Compare, OrderingAndEq) {
	// Arrange
	const R a{1,2};
	const R b{2,4};
	const R c{1,3};
	const R d{1,2};
	const R e{-1,2};
	const R f{0};
	const R g{2};
	const R h{3,2};
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
	R a{0, -5};
	// Assert
	EXPECT_EQ(a.get_numerator(), 0);
	EXPECT_EQ(a.get_denominator(), 1);
	// Act
	a += R{0, 7};
	// Assert
	EXPECT_EQ(a.get_numerator(), 0);
	EXPECT_EQ(a.get_denominator(), 1);
}

TEST(Rational_EdgeCases, NegativeDenominatorNormalized) {
	// Arrange & Act
	R a{1, -3};
	// Assert
	EXPECT_EQ(a.get_numerator(), -1);
	EXPECT_EQ(a.get_denominator(), 3);
}

TEST(Rational_Chains, LongExpressionStaysNormalized) {
	// Arrange
	R r{3, 4};
	// Act
	r = r + R{5, 6} - R{7, 8} + 2;
	// Assert
	EXPECT_EQ(r.as_string(), "65/24");
	// Act
	r = (r * R{9, 7}) / R{3, 1};
	// Assert
	EXPECT_EQ(r.as_string(), "65/56");
}

TEST(Rational_CopySemantics, CopyAndAssign) {
	// Arrange
	R a{4, 6};
	// Act
	R b{a};
	// Assert
	EXPECT_EQ(b.as_string(), "2/3");
	// Arrange
	R c;
	// Act
	c = a;
	// Assert
	EXPECT_EQ(c.as_string(), "2/3");
}

TEST(Rational_Assignment, SelfAssignmentNoChange) {
	// Arrange
	R a{5, 10};
	// Act
	a = a;
	// Assert
	EXPECT_EQ(a.as_string(), "1/2");
}

TEST(Rational_Compare, NeqAndLeGe) {
	// Arrange
	const R a{1,2};
	const R b{2,3};
	const R c{3,3};
	const R d{1,1};
	// Act
	const bool ne_ab = (a != b);
	const bool le_ab = (a <= b);
	const bool ge_cd = (c >= d);
	// Assert
	EXPECT_TRUE(ne_ab);
	EXPECT_TRUE(le_ab);
	EXPECT_TRUE(ge_cd);
}

TEST(Rational_Inverse, InvertNormalAndErrors) {
	R a{2, 3};
	a.inverse();
	EXPECT_EQ(a.as_string(), "3/2");
	R b{5};
	b.inverse();
	EXPECT_EQ(b.as_string(), "1/5");
	R z{0};
	EXPECT_THROW(z.inverse(), division_by_zero_error);
}


TEST(Rational_Matrix, BasicOpsAndInverse) {
	using M = ::matrix_t<int>;
	using RM = ::rational_t<M>;
	RM r{ M{2}, M{3} };
	EXPECT_EQ(r.as_string(), "[2]/[3]");
	r.inverse();
	EXPECT_EQ(r.as_string(), "[3]/[2]");
	RM s{ M{1}, M{2} };
	auto t = r * s; // (3/2)*(1/2) = 3/4 element-wise
	EXPECT_EQ(t.as_string(), "[3]/[4]");
}