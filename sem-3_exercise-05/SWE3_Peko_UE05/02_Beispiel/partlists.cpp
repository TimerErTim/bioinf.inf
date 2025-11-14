#include "pch.h"
/**
 * @file partlists.cpp
 * @brief Factory helpers and internal leaf implementation for PartLists.
 *
 * Contains:
 * - A private Leaf class implementing Part for simple leaf nodes.
 * - Factories `makePart` and `makeComposite` for convenient creation.
 */
#include "partlists.h"

using namespace PartLists;

// Keep factories in this TU to avoid header-only leaf implementation leaking.
// Rationale: encapsulates the internal Leaf class to a single translation unit,
// minimizing ODR risks and compile-time exposure while keeping the public API small.

/**
 * @brief Create a simple leaf Part with the given name.
 * @param name Non-empty part name.
 * @return std::unique_ptr<Part> New leaf instance.
 */
std::unique_ptr<Part> PartLists::makePart(std::string name) {
	/**
	 * @brief Simple leaf Part implementation used by the factory.
	 * Note: kept local to this TU to avoid exporting an extra symbol/type
	 * that is not required by the public API.
	 */
	class Leaf final : public Part {
	public:
		/**
		 * @brief Construct a leaf part.
		 * @param n Non-empty name.
		 * @throws std::invalid_argument If the name is empty (enforced by Part).
		 */
		explicit Leaf(std::string n) : Part(std::move(n)) {}
		/**
		 * @brief Deep clone the leaf.
		 * @return std::unique_ptr<Part> New leaf with the same name.
		 */
		std::unique_ptr<Part> clone() const override {
			return std::make_unique<Leaf>(getName());
		}
		/**
		 * @brief Accept a formatter to print this leaf.
		 * @param formatter Formatter to use.
		 * @param os Output stream.
		 * @param depth Optional indentation depth (ignored).
		 */
		void accept(Formatter& formatter, std::ostream& os, int depth) const override {
			(void)depth;
			// We intentionally delegate formatting control to the Formatter strategy.
			formatter.printParts(*this, os);
		}
	};
	return std::make_unique<Leaf>(std::move(name));
}

/**
 * @brief Create a CompositePart with the given name.
 * @param name Non-empty composite name.
 * @return std::unique_ptr<CompositePart> New composite instance.
 */
std::unique_ptr<CompositePart> PartLists::makeComposite(std::string name) {
	return std::make_unique<CompositePart>(std::move(name));
}



