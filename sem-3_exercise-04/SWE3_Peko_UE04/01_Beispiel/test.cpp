#include "pch.h"
#include "rational_t.hpp"
#include "matrix_t.hpp"
#include "errors.hpp"
#include <sstream>
#include <type_traits>
#include <gtest/gtest.h>

// Tests are structured (by subtask), isolated (one behavior per test), using AAA.

namespace subtask_3 {
// value_type export

static_assert(std::is_same_v<rational_t<int>::value_type, int>, "value_type must be int for rational_t<int>");
static_assert(std::is_same_v<rational_t<long>::value_type, long>, "value_type must be long for rational_t<long>");

}

namespace subtask_2 {
// Generic and construction/normalization (int)

TEST(RationalInt_Construct, NormalizesFraction) {
	// Arrange
	rational_t<int> r(2, 4);
	// Act
	// Assert
	EXPECT_EQ(r.as_string(), "<1/2>");
}

TEST(RationalInt_Construct, NormalizesSign) {
	// Arrange
	rational_t<int> r(1, -2);
	// Act
	// Assert
	EXPECT_EQ(r.as_string(), "<-1/2>");
}

TEST(RationalInt_Construct, DoubleNegative) {
	// Arrange
	rational_t<int> r(-1, -2);
	// Act
	// Assert
	EXPECT_EQ(r.as_string(), "<1/2>");
}

TEST(RationalInt_Construct, ZeroCanonical) {
	// Arrange
	rational_t<int> r(0, 5);
	// Act
	// Assert
	EXPECT_EQ(r.as_string(), "<0>");
}

}

namespace subtask_1 {
// Inverse-Methode 

TEST(RationalInt_Inverse, Inverts) {
	// Arrange
	rational_t<int> r(3, 5);
	// Act
	r.inverse();
	// Assert
	EXPECT_EQ(r.as_string(), "<5/3>");
}

TEST(RationalInt_Inverse, ZeroThrows) {
	// Arrange
	rational_t<int> r(0, 1);
	// Act + Assert
	EXPECT_THROW(r.inverse(), division_by_zero_error);
}

}

namespace subtask_7 {
// Operations

TEST(RationalInt_Arithmetic, AddAdds) {
	// Arrange
	rational_t<int> a(1, 2), b(1, 3);
	// Act
	rational_t<int> s = a + b;
	// Assert
	EXPECT_EQ(s.as_string(), "<5/6>");
}

TEST(RationalInt_Arithmetic, SubSubtracts) {
	// Arrange
	rational_t<int> a(3, 4), b(1, 2);
	// Act
	rational_t<int> d = a - b;
	// Assert
	EXPECT_EQ(d.as_string(), "<1/4>");
}

TEST(RationalInt_Arithmetic, MulMultiplies) {
	// Arrange
	rational_t<int> a(2, 3), b(9, 4);
	// Act
	rational_t<int> p = a * b;
	// Assert
	EXPECT_EQ(p.as_string(), "<3/2>");
}

TEST(RationalInt_Arithmetic, DivDivides) {
	// Arrange
	rational_t<int> a(7, 5), b(7, 10);
	// Act
	rational_t<int> q = a / b;
	// Assert
	EXPECT_EQ(q.as_string(), "<2>");
}

TEST(RationalInt_Comparison, EqEqualEvenIfNotReduced) {
	// Arrange
	rational_t<int> a(2, 4), b(1, 2);
	// Act
	bool eq = (a == b);
	// Assert
	EXPECT_TRUE(eq);
}

TEST(RationalInt_Comparison, LtLessThan) {
	// Arrange
	rational_t<int> a(1, 3), b(1, 2);
	// Act
	bool lt = (a < b);
	// Assert
	EXPECT_TRUE(lt);
}

TEST(RationalInt_Stream, ReadFraction) {
	// Arrange
	std::istringstream in("<3/4>");
	rational_t<int> r;
	// Act
	in >> r;
	// Assert
	EXPECT_EQ(r.as_string(), "<3/4>");
}

TEST(RationalInt_Stream, ReadInteger) {
	// Arrange
	std::istringstream in("<5>");
	rational_t<int> r;
	// Act
	in >> r;
	// Assert
	EXPECT_EQ(r.as_string(), "<5>");
}

TEST(RationalInt_Stream, WriteRoundtrip) {
	// Arrange
	rational_t<int> a(7, 8);
	std::ostringstream out;
	// Act
	out << a;
	// Assert
	EXPECT_EQ(out.str(), std::string("<7/8>"));
}

TEST(RationalInt_Stream, ParseMissingOpenFails) {
	// Arrange
	std::istringstream in("3/4>");
	rational_t<int> r;
	// Act
	in >> r;
	// Assert
	EXPECT_TRUE(in.fail());
}

TEST(RationalInt_Stream, ParseMissingCloseFails) {
	// Arrange
	std::istringstream in("<3/4");
	rational_t<int> r;
	// Act
	in >> r;
	// Assert
	EXPECT_TRUE(in.fail());
}

TEST(RationalInt_Stream, ParseNonNumericFails) {
	// Arrange
	std::istringstream in("<a/b>");
	rational_t<int> r;
	// Act
	in >> r;
	// Assert
	EXPECT_TRUE(in.fail());
}

TEST(RationalInt_Stream, ParseZeroDenThrows) {
	// Arrange
	std::istringstream in("<1/0>");
	rational_t<int> r;
	// Act + Assert
	EXPECT_THROW(in >> r, invalid_rational_error);
}

TEST(RationalInt_Error, ConstructZeroDen) {
	// Arrange + Act + Assert
	EXPECT_THROW((void)rational_t<int>(1, 0), invalid_rational_error);
}

TEST(RationalInt_Error, DivideByZeroRational) {
	// Arrange
	rational_t<int> a(1, 2);
	rational_t<int> z(0, 1);
	// Act + Assert
	EXPECT_THROW(a /= z, division_by_zero_error);
}

TEST(RationalInt_Mixed, Add) {
	// Arrange
	rational_t<int> r(3, 4);
	// Act
	auto s = 2 + r;
	// Assert
	EXPECT_EQ(s.as_string(), "<11/4>");
}

TEST(RationalInt_Mixed, Sub) {
	// Arrange
	rational_t<int> r(3, 4);
	// Act
	auto d = 2 - r;
	// Assert
	EXPECT_EQ(d.as_string(), "<5/4>");
}

TEST(RationalInt_Mixed, Mul) {
	// Arrange
	rational_t<int> r(3, 4);
	// Act
	auto p = 2 * r;
	// Assert
	EXPECT_EQ(p.as_string(), "<3/2>");
}

TEST(RationalInt_Mixed, Div) {
	// Arrange
	rational_t<int> r(3, 4);
	// Act
	auto q = 2 / r;
	// Assert
	EXPECT_EQ(q.as_string(), "<8/3>");
}

}

namespace subtask_4 {
// Verallgemeinerung – long and matrix_t<int>

TEST(RationalLong_Construct, Normalization) {
	// Arrange
	rational_t<long> a(10L, 20L);
	// Act
	// Assert
	EXPECT_EQ(a.as_string(), "<1/2>");
}

TEST(RationalLong_Construct, DoubleNegative) {
	// Arrange
	rational_t<long> b(-14L, -21L);
	// Act
	// Assert
	EXPECT_EQ(b.as_string(), "<2/3>");
}

TEST(RationalLong_Arithmetic, Add) {
	// Arrange
	rational_t<long> a(123456789L, 100000000L);
	rational_t<long> b(1L, 10L);
	// Act
	rational_t<long> s = a + b;
	// Assert
	EXPECT_EQ(s.as_string(), "<133456789/100000000>");
}

TEST(RationalLong_Arithmetic, MulLarge) {
	// Arrange
	rational_t<long long> a(122337203685477580LL, 2LL);
	rational_t<long long> b(2LL, 3LL);
	// Act
	rational_t<long long> p = a * b;
	// Assert
	EXPECT_EQ(p.as_string(), "<122337203685477580/3>");
}

TEST(RationalLong_IO, Write) {
	// Arrange
	rational_t<long> a(-5000000000L, 10000000000L);
	std::ostringstream out;
	// Act
	out << a;
	// Assert
	EXPECT_EQ(out.str(), "<-1/2>");
}

TEST(RationalLong_Inverse, Inverse) {
	// Arrange
	rational_t<long> x(7L, -3L);
	// Act
	x.inverse();
	// Assert
	EXPECT_EQ(x.as_string(), "<-3/7>");
}

TEST(RationalLong_Error, ConstructZeroDen) {
	// Arrange + Act + Assert
	EXPECT_THROW((void)rational_t<long>(1L, 0L), invalid_rational_error);
}

TEST(RationalMatrix_Construct, InvalidDenThrows) {
	// Arrange + Act + Assert
	using M = matrix_t<int>;
	EXPECT_THROW((void)rational_t<M>(M{1}, M{0}), invalid_rational_error);
}

TEST(RationalMatrix_Comparison, EqCrossMultiplication) {
	// Arrange
	using M = matrix_t<int>;
	rational_t<M> a(M{2}, M{4});
	rational_t<M> b(M{1}, M{2});
	// Act
	bool eq = (a == b);
	// Assert
	EXPECT_TRUE(eq);
}

TEST(RationalMatrix_Arithmetic, AddSumEqualsOne) {
	// Arrange
	using M = matrix_t<int>;
	rational_t<M> a(M{2}, M{4});
	rational_t<M> b(M{1}, M{2});
	// Act
	rational_t<M> c = a + b;
	// Assert (equality via cross multiplication with 1/1)
	EXPECT_TRUE(c == rational_t<M>(M{1}, M{1}));
}

TEST(RationalMatrix_Arithmetic, AddLhsBareMatrix) {
	// Arrange
	using M = matrix_t<int>;
	M a{ 1 };
	rational_t<M> b(M{ 1 }, M{ 2 });
	// Act
	rational_t<M> c = a + b;
	// Assert
	EXPECT_EQ(c, rational_t<M>(M{ 3 }, M{ 2 }));
}

TEST(RationalMatrix_Inverse, Swaps) {
	// Arrange
	using M = matrix_t<int>;
	rational_t<M> d(M{5}, M{7});
	// Act
	d.inverse();
	// Assert
	EXPECT_TRUE(d == rational_t<M>(M{7}, M{5}));
}

TEST(RationalMatrix_Stream, WriteFormatting) {
	// Arrange
	using M = matrix_t<int>;
	rational_t<M> a(M{2}, M{4});
	std::ostringstream os;
	// Act
	os << a;
	// Assert (no reduction enforced for matrix domain)
	EXPECT_EQ(os.str(), std::string("<[1]/[2]>") );
}

TEST(RationalMatrix_Stream, ReadFraction) {
	// Arrange
	using M = matrix_t<int>;
	std::istringstream in("<[2]/[4]>");
	rational_t<M> r(M{1}, M{1}); // seed non-zero to ensure replacement
	// Act
	in >> r;
	// Assert (expects reduction if normalization supports it)
	EXPECT_EQ(r.as_string(), std::string("<[1]/[2]>") );
}

TEST(RationalMatrix_Stream, ReadInteger) {
	// Arrange
	using M = matrix_t<int>;
	std::istringstream in("<[5]>");
	rational_t<M> r;
	// Act
	in >> r;
	// Assert
	EXPECT_EQ(r.as_string(), std::string("<[5]>") );
}

TEST(RationalMatrix_Stream, ParseZeroDenThrows) {
	// Arrange
	using M = matrix_t<int>;
	std::istringstream in("<[1]/[0]>");
	rational_t<M> r;
	// Act + Assert
	EXPECT_THROW(in >> r, invalid_rational_error);
}

}
