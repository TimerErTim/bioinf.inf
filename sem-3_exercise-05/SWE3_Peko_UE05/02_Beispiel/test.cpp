#include "pch.h"
#include "partlists.h"

#include <sstream>
#include <fstream>
#include <cstdio>

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

// ---- Additional Coverage & Edge Cases ----

// Helper: sanitize file path like Part::defaultFilePath
static std::string sanitizePath(std::string name) {
	for (char& ch : name) {
		if (!std::isalnum(static_cast<unsigned char>(ch))) ch = '_';
	}
	return name + ".plist";
}

TEST(PartLists, EmptyCompositeFormatting) {
	// Arrange
	auto empty = makeComposite("Empty");
	HierarchyFormatter h;
	SetFormatter s;
	std::ostringstream osh, oss;

	// Act
	h.printParts(*empty, osh);
	s.printParts(*empty, oss);

	// Assert
	EXPECT_EQ(osh.str(), "Empty\n");
	EXPECT_EQ(oss.str(), "Empty:\n");
}

TEST(PartLists, SetFormatterDeterministicOrder) {
	// Arrange
	auto root = makeComposite("Root");
	auto a = makeComposite("A");
	auto b = makeComposite("B");
	// Insertion order of leaves:
	// A -> x, y
	a->addPart(*makePart("x"));
	a->addPart(*makePart("y"));
	// B -> y, z, x
	b->addPart(*makePart("y"));
	b->addPart(*makePart("z"));
	b->addPart(*makePart("x"));
	root->addPart(std::move(a));
	root->addPart(std::move(b));

	SetFormatter fmt;
	std::ostringstream os;

	// Act
	fmt.printParts(*root, os);

	// Assert (order is first occurrence: x, y, z)
	EXPECT_EQ(os.str(),
		"Root:\n"
		"  2 x\n"
		"  2 y\n"
		"  1 z\n");
}

TEST(PartLists, Equality_OrderMatters) {
	// Arrange
	auto r1 = makeComposite("R");
	auto r2 = makeComposite("R");
	r1->addPart(*makePart("x"));
	r1->addPart(*makePart("y"));
	r2->addPart(*makePart("y"));
	r2->addPart(*makePart("x")); // different order

	// Act
	bool eq = r1->equalsTo(*r2);

	// Assert
	EXPECT_FALSE(eq);
}

TEST(PartLists, CloneDeepAndMutationIsolation) {
	// Arrange
	auto root = buildFurniture();
	auto cloned = root->clone();

	// Act
	// Mutate original (add one more leaf under last child "Tisch")
	auto* tisch = dynamic_cast<CompositePart*>(root->getParts().back());
	ASSERT_NE(tisch, nullptr);
	tisch->addPart(*makePart("EXTRA"));

	// Assert
	EXPECT_FALSE(root->equalsTo(*cloned));
}

TEST(PartLists, AddPartNullptrThrows) {
	// Arrange
	auto c = makeComposite("C");
	std::unique_ptr<Part> nullp;

	// Act & Assert
	EXPECT_THROW(c->addPart(std::move(nullp)), std::invalid_argument);
}

TEST(PartLists, AcceptCallsFormatter) {
	// Arrange
	struct CountingFormatter : Formatter {
		int calls = 0;
		void printParts(Part const& p, std::ostream& os) override {
			(void)p;
			(void)os;
			++calls;
		}
	};
	auto leaf = makePart("Leaf");
	auto comp = makeComposite("Comp");
	comp->addPart(*leaf);
	CountingFormatter fmt;
	std::ostringstream os;

	// Act
	leaf->accept(fmt, os);
	comp->accept(fmt, os);

	// Assert
	EXPECT_EQ(fmt.calls, 2);
}

TEST(PartLists, Persistence_WritesExpectedHeaders) {
	// Arrange
	auto leaf = makePart("Persist Leaf");
	auto comp = makeComposite("Persist Comp");
	comp->addPart(*makePart("L1"));

	// Act
	leaf->store();
	comp->store();

	// Assert
	{
		std::ifstream in(sanitizePath("Persist Leaf"));
		ASSERT_TRUE(in.is_open());
		std::string line;
		std::getline(in, line);
		EXPECT_EQ(line, "P|Persist Leaf");
		in.close();
	}
	{
		std::ifstream in(sanitizePath("Persist Comp"));
		ASSERT_TRUE(in.is_open());
		std::string line;
		std::getline(in, line);
		EXPECT_EQ(line, "C|Persist Comp");
		in.close();
	}

	// Cleanup
	std::remove(sanitizePath("Persist Leaf").c_str());
	std::remove(sanitizePath("Persist Comp").c_str());
}

TEST(PartLists, LoadRejectsInvalidRootForComposite) {
	// Arrange
	auto comp = makeComposite("Invalid Root");
	// Write an invalid file header for a composite
	{
		std::ofstream out(sanitizePath("Invalid Root"), std::ios::trunc);
		out << "P|Invalid Root\n";
		out.close();
	}

	// Act & Assert
	EXPECT_THROW(comp->load(), std::runtime_error);

	// Cleanup
	std::remove(sanitizePath("Invalid Root").c_str());
}

TEST(PartLists, DeepHierarchyFormatting) {
	// Arrange
	auto root = makeComposite("Root");
	CompositePart* current = root.get();
	for (int i = 1; i <= 20; ++i) {
		auto next = makeComposite("N" + std::to_string(i));
		CompositePart* nextPtr = next.get();
		current->addPart(std::move(next));
		current = nextPtr;
	}
	current->addPart(*makePart("Leaf"));

	HierarchyFormatter fmt;
	std::ostringstream os;

	// Act
	fmt.printParts(*root, os);
	std::string s = os.str();

	// Assert
	EXPECT_NE(s.find("Root\n"), std::string::npos);
	EXPECT_NE(s.find("Leaf"), std::string::npos);
	// The output should contain multiple indented levels; quick sanity check
	EXPECT_NE(s.find("                    Leaf"), std::string::npos); // 20*2 spaces
}

