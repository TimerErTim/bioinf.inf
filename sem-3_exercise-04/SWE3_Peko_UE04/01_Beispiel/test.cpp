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

// ---------------- Additional extensive tests ----------------

TEST(RationalInt_IO, RoundtripAndWhitespace) {
	// Roundtrip multiple rationals and whitespace handling around tokens
	std::ostringstream out;
	rational_t<int> a(7, 8), b(5), c(-3, 4);
	out << a << ' ' << b << '\n' << c;
	EXPECT_EQ(out.str(), std::string("<7/8> <5>\n<-3/4>")) << "Formatted output mismatch";

	std::istringstream in(out.str());
	rational_t<int> ra, rb, rc;
	in >> ra >> rb >> rc;
	EXPECT_EQ(ra.as_string(), "<7/8>");
	EXPECT_EQ(rb.as_string(), "<5>");
	EXPECT_EQ(rc.as_string(), "<-3/4>");
}

TEST(RationalInt_ParseFailures, VariousMalformedInputs) {
	{
		std::istringstream in("3/4>"); // missing '<'
		rational_t<int> r;
		in >> r;
		EXPECT_TRUE(in.fail()) << "Missing '<' should set failbit";
	}
	{
		std::istringstream in("<3/4"); // missing '>'
		rational_t<int> r;
		in >> r;
		EXPECT_TRUE(in.fail()) << "Missing '>' should set failbit";
	}
	{
		std::istringstream in("<a/b>"); // non-numeric
		rational_t<int> r;
		in >> r;
		EXPECT_TRUE(in.fail()) << "Non-numeric should set failbit";
	}
}

TEST(RationalInt_Properties, SignsAndPredicates) {
	rational_t<int> a(-1, 2);
	EXPECT_TRUE(a.is_negative());
	EXPECT_FALSE(a.is_positive());
	EXPECT_FALSE(a.is_zero());

	rational_t<int> z(0);
	EXPECT_TRUE(z.is_zero());
	EXPECT_FALSE(z.is_positive());
	EXPECT_FALSE(z.is_negative());
}

TEST(RationalInt_MixedOps, AllOperatorsWithIntLHS) {
	rational_t<int> r(3, 4);
	EXPECT_EQ((2 + r).as_string(), "<11/4>");
	EXPECT_EQ((2 - r).as_string(), "<5/4>");
	EXPECT_EQ((2 * r).as_string(), "<3/2>");
	EXPECT_EQ((2 / r).as_string(), "<8/3>");
}

TEST(RationalInt_ComparisonMore, CrossMultiplicationEquality) {
	// Ensure equality works even if not reduced internally
	rational_t<int> a(100, 250); // 2/5
	rational_t<int> b(2, 5);
	EXPECT_TRUE(a == b);
	EXPECT_FALSE(a < b);
	EXPECT_FALSE(a > b);
}

TEST(RationalLong, ConstructionNormalizationAndArithmetic) {
	rational_t<long> a(10L, 20L);
	EXPECT_EQ(a.as_string(), "<1/2>");

	rational_t<long> b(-14L, -21L); // -> 2/3
	EXPECT_EQ(b.as_string(), "<2/3>");

	rational_t<long> sum = rational_t<long>(123456789L, 100000000L) + rational_t<long>(1L, 10L);
	// 123456789/100000000 + 1/10 = 123456789/100000000 + 10/100 = 123456889/100000000 => reduces by gcd 1
	EXPECT_EQ(sum.as_string(), "<133456789/100000000>");

	rational_t<long long> prod = rational_t<long long>(122337203685477580L, 2L) * rational_t<long long>(2L, 3L); // careful but still within range
	EXPECT_EQ(prod.as_string(), "<122337203685477580/3>");
}

TEST(RationalLong_IO, Roundtrip) {
	std::ostringstream out;
	rational_t<long> a(-5000000000L, 10000000000L); // -> -1/2
	out << a;
	EXPECT_EQ(out.str(), "<-1/2>");
	std::istringstream in(out.str());
	rational_t<long> r;
	in >> r;
	EXPECT_EQ(r.as_string(), "<-1/2>");
}

TEST(RationalLong_EdgeCases, InverseAndDivisionByZero) {
	rational_t<long> x(7L, -3L); // -> -7/3
	x.inverse(); // -> -3/7
	EXPECT_EQ(x.as_string(), "<-3/7>");
	EXPECT_THROW((void)rational_t<long>(1L, 0L), invalid_rational_error);
}

TEST(RationalMatrix_Errors, InvalidDenominatorThrows) {
	using M = matrix_t<int>;
	EXPECT_THROW((void)rational_t<M>(M{1}, M{0}), invalid_rational_error);
}

TEST(RationalMatrix_Formatting, OutputFormatting) {
	using M = matrix_t<int>;
	rational_t<M> a(M{2}, M{4});
	std::ostringstream os;
	os << a;
	EXPECT_EQ(os.str(), std::string("<[1]/[2]>") );
}

TEST(RationalMatrix_More, SumEqualsOneAndInverse) {
	using M = matrix_t<int>;
	rational_t<M> a(M{2}, M{4});
	rational_t<M> b(M{1}, M{2});
	EXPECT_TRUE(a == b);
	rational_t<M> c = a + b; // [2]/[4] + [1]/[2] = [4]/[4] => [1]
	EXPECT_TRUE(c == rational_t<M>(M{1}, M{1}));

	rational_t<M> d(M{5}, M{7});
	d.inverse();
	EXPECT_TRUE(d == rational_t<M>(M{7}, M{5}));
}

