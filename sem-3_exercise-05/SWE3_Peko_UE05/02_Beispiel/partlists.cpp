#include "pch.h"
#include "partlists.h"

using namespace PartLists;

// Keep factories in this TU to avoid header-only leaf implementation leaking.

std::unique_ptr<Part> PartLists::makePart(std::string name) {
	class Leaf final : public Part {
	public:
		explicit Leaf(std::string n) : Part(std::move(n)) {}
		std::unique_ptr<Part> clone() const override {
			return std::make_unique<Leaf>(getName());
		}
		void accept(Formatter& formatter, std::ostream& os, int depth) const override {
			(void)depth;
			formatter.printParts(*this, os);
		}
	};
	return std::make_unique<Leaf>(std::move(name));
}

std::unique_ptr<CompositePart> PartLists::makeComposite(std::string name) {
	return std::make_unique<CompositePart>(std::move(name));
}



