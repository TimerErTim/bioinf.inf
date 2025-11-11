#include "pch.h"
#include "partlists.h"

#include <sstream>

using namespace PartLists;

// Tests follow the AAA (Arrange-Act-Assert) principle.

/**
 * Helper to build the furniture example:
 *
 * Sitzgarnitur
 *   Sessel
 *     Bein (klein) x4
 *     Sitzfläche
 *   Sessel
 *     Bein (klein) x4
 *     Sitzfläche
 *   Tisch
 *     Bein (groß) x4
 *     Tischfläche
 */
static std::unique_ptr<CompositePart> buildFurniture() {
	auto sitzgarnitur = makeComposite("Sitzgarnitur");

	auto makeSessel = []() {
		auto s = makeComposite("Sessel");
		for (int i = 0; i < 4; ++i) s->addPart(*makePart("Bein (klein)"));
		s->addPart(*makePart("Sitzfläche"));
		return s;
	};

	auto sessel1 = makeSessel();
	auto sessel2 = makeSessel();

	auto tisch = makeComposite("Tisch");
	for (int i = 0; i < 4; ++i) tisch->addPart(*makePart("Bein (groß)"));
	tisch->addPart(*makePart("Tischfläche"));

	sitzgarnitur->addPart(std::move(sessel1));
	sitzgarnitur->addPart(std::move(sessel2));
	sitzgarnitur->addPart(std::move(tisch));

	return sitzgarnitur;
}

TEST(PartLists, PartRejectsEmptyName) {
	// Arrange
	// Act & Assert
	EXPECT_THROW(makePart(""), std::invalid_argument);
	EXPECT_THROW(makeComposite(""), std::invalid_argument);
}

TEST(PartLists, HierarchyFormatter_PrintsIndentedTree) {
	// Arrange
	auto root = buildFurniture();
	HierarchyFormatter fmt;
	std::ostringstream os;

	// Act
	fmt.printParts(*root, os);

	// Assert
	std::string expected =
		"Sitzgarnitur\n"
		"  Sessel\n"
		"    Bein (klein)\n"
		"    Bein (klein)\n"
		"    Bein (klein)\n"
		"    Bein (klein)\n"
		"    Sitzfläche\n"
		"  Sessel\n"
		"    Bein (klein)\n"
		"    Bein (klein)\n"
		"    Bein (klein)\n"
		"    Bein (klein)\n"
		"    Sitzfläche\n"
		"  Tisch\n"
		"    Bein (groß)\n"
		"    Bein (groß)\n"
		"    Bein (groß)\n"
		"    Bein (groß)\n"
		"    Tischfläche\n";

	EXPECT_EQ(os.str(), expected);
}

TEST(PartLists, SetFormatter_PrintsAggregatedMultiset) {
	// Arrange
	auto root = buildFurniture();
	SetFormatter fmt;
	std::ostringstream os;

	// Act
	fmt.printParts(*root, os);

	// Assert
	std::string out = os.str();
	EXPECT_NE(out.find("Sitzgarnitur:"), std::string::npos);
	// Expect insertion-order counts based on traversal
	EXPECT_NE(out.find("  8 Bein (klein)"), std::string::npos);
	EXPECT_NE(out.find("  2 Sitzfläche"), std::string::npos);
	EXPECT_NE(out.find("  4 Bein (groß)"), std::string::npos);
	EXPECT_NE(out.find("  1 Tischfläche"), std::string::npos);
}

TEST(PartLists, EqualsTo_ComparesStructureAndNames) {
	// Arrange
	auto a = buildFurniture();
	auto b = buildFurniture();

	// Act
	bool same = a->equalsTo(*b);

	// Assert
	EXPECT_TRUE(same);

	// Mutate b slightly
	auto extra = makePart("Bein (klein)");
	// add to the last child (Tisch) to break equality
	auto parts = b->getParts();
	auto* tisch = dynamic_cast<CompositePart*>(parts.back());
	ASSERT_NE(tisch, nullptr);
	tisch->addPart(*extra);

	EXPECT_FALSE(a->equalsTo(*b));
}

TEST(PartLists, StoreAndLoad_DoNotThrow) {
	// Arrange
	auto root = buildFurniture();

	// Act & Assert
	EXPECT_NO_THROW(root->store());
	EXPECT_NO_THROW(root->load()); // no full rebuild, but should validate header
}

