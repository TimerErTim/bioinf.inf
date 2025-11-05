#include "pch.h"
#include "rational_t.hpp"
#include "matrix_t.hpp"
#include "errors.hpp"
#include <sstream>
#include <gtest/gtest.h>

// NOTE: Tests include clear names, expectation vs. actual output via messages.

// ---- Tests for rational_t<int> ----

TEST(RationalInt, ConstructionAndNormalization) {
	rational_t<int> a(2, 4);
	EXPECT_EQ(a.as_string(), "<1/2>") << "Construction/Normalization: expected <1/2>, actual " << a.as_string();

	rational_t<int> b(1, -2);
	EXPECT_EQ(b.as_string(), "<-1/2>") << "Sign normalization: expected <-1/2>, actual " << b.as_string();

	rational_t<int> c(-1, -2);
	EXPECT_EQ(c.as_string(), "<1/2>") << "Double negative: expected <1/2>, actual " << c.as_string();

	rational_t<int> d(0, 5);
	EXPECT_EQ(d.as_string(), "<0>") << "Zero canonicalization: expected <0>, actual " << d.as_string();
}

TEST(RationalInt, Inverse) {
	rational_t<int> a(3, 5);
	a.inverse();
	EXPECT_EQ(a.as_string(), "<5/3>") << "Inverse: expected <5/3>, actual " << a.as_string();

	rational_t<int> z(0, 1);
	EXPECT_THROW(z.inverse(), division_by_zero_error) << "Inverse of zero must throw division_by_zero_error";
}

TEST(RationalInt, Arithmetic) {
	rational_t<int> a(1, 2);
	rational_t<int> b(1, 3);

	a += b; // 1/2 + 1/3 = 5/6
	EXPECT_EQ(a.as_string(), "<5/6>") << "+= result: expected <5/6>, actual " << a.as_string();

	rational_t<int> c = rational_t<int>(3, 4) - rational_t<int>(1, 2); // 1/4
	EXPECT_EQ(c.as_string(), "<1/4>") << "- result: expected <1/4>, actual " << c.as_string();

	rational_t<int> d = rational_t<int>(2, 3) * rational_t<int>(9, 4); // 3/2
	EXPECT_EQ(d.as_string(), "<3/2>") << "* result: expected <3/2>, actual " << d.as_string();

	rational_t<int> e = rational_t<int>(7, 5) / rational_t<int>(7, 10); // 2
	EXPECT_EQ(e.as_string(), "<2>") << "/ result: expected <2>, actual " << e.as_string();

	// Chained expression: (1/2 + 1/3) * (3/5) - 1/10 = (5/6)*(3/5) - 1/10 = (1/2) - 1/10 = 2/5
	rational_t<int> chain = (rational_t<int>(1, 2) + rational_t<int>(1, 3)) * rational_t<int>(3, 5) - rational_t<int>(1, 10);
	EXPECT_EQ(chain.as_string(), "<2/5>") << "Chained arithmetic: expected <2/5>, actual " << chain.as_string();
}

TEST(RationalInt, Comparison) {
	rational_t<int> a(2, 4);
	rational_t<int> b(1, 2);
	EXPECT_TRUE(a == b) << "Equality via cross-multiplication failed: " << a.as_string() << " vs " << b.as_string();
	EXPECT_FALSE(a < b) << "Ordering: 1/2 < 1/2 should be false";
	EXPECT_TRUE(rational_t<int>(1, 3) < rational_t<int>(1, 2)) << "Ordering: 1/3 < 1/2 should be true";
}

TEST(RationalInt, StreamIO) {
	{
		std::istringstream in("<3/4>");
		rational_t<int> r;
		in >> r;
		EXPECT_EQ(r.as_string(), "<3/4>") << "Stream parse <n/d>: expected <3/4>, actual " << r.as_string();
	}
	{
		std::istringstream in("<5>");
		rational_t<int> r;
		in >> r;
		EXPECT_EQ(r.as_string(), "<5>") << "Stream parse <n>: expected <5>, actual " << r.as_string();
	}
	{
		std::istringstream bad("<1/0>");
		rational_t<int> r;
		EXPECT_THROW(bad >> r, invalid_rational_error) << "Parsing <1/0> must throw invalid_rational_error";
	}
}

TEST(RationalInt, MixedIntOnLeftHandSide) {
	rational_t<int> r(1, 2);
	auto s = 2 + r; // 5/2
	EXPECT_EQ(s.as_string(), "<5/2>") << "int + rational: expected <5/2>, actual " << s.as_string();
}

TEST(RationalInt, DivisionByZeroRationalThrows) {
	EXPECT_THROW((void)rational_t<int>(1, 0), invalid_rational_error) << "Denominator zero must throw invalid_rational_error";
	EXPECT_THROW(
		[]() {
			rational_t<int> a(1, 2);
			rational_t<int> z(0, 1);
			a /= z;
		}(),
		division_by_zero_error) << "Division by zero rational must throw division_by_zero_error";
}

// ---- Tests for rational_t<matrix_t<int>> (1×1 matrices) ----

TEST(RationalMatrix, EqualityByCrossMultiplication) {
	using M = matrix_t<int>;
	rational_t<M> a(M{2}, M{4});
	rational_t<M> b(M{1}, M{2});
	EXPECT_TRUE(a == b) << "[2]/[4] should equal [1]/[2] via cross-multiplication";
}

TEST(RationalMatrix, ArithmeticAndInverse) {
	using M = matrix_t<int>;
	rational_t<M> a(M{2}, M{3});
	rational_t<M> b(M{3}, M{4});
	rational_t<M> prod = a * b; // [6]/[12]
	EXPECT_TRUE(prod == rational_t<M>(M{6}, M{12})) << "Matrix product as rationals should match expected [6]/[12]";

	prod.inverse(); // [12]/[6]
	EXPECT_TRUE(prod == rational_t<M>(M{12}, M{6})) << "Inverse should swap numerator/denominator for matrix domain";
}

