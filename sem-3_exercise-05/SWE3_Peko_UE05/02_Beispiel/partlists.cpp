#include "pch.h"
#include "partlists.h"

#include <fstream>
#include <sstream>
#include <cctype>

namespace PartLists {

	// ---------- Part ----------

	Part::Part(std::string name)
		: m_name(std::move(name)) {
		if (m_name.empty()) {
			throw std::invalid_argument("Part name must not be empty");
		}
	}

	bool Part::equalsTo(Part const& other) const {
		return this->getName() == other.getName();
	}

	std::string Part::defaultFilePath() const {
		// Create a filename that is safe enough for demonstration (letters/digits/underscore)
		std::string base = getName();
		for (char& ch : base) {
			if (!std::isalnum(static_cast<unsigned char>(ch))) {
				ch = '_';
			}
		}
		return base + ".plist";
	}

	void Part::store() const {
		// Default leaf serialization: "P|<name>\n"
		std::ofstream out(defaultFilePath(), std::ios::trunc);
		if (!out) {
			throw std::runtime_error("Failed to open file for storing Part");
		}
		out << "P|" << getName() << "\n";
	}

	void Part::load() {
		// Default leaf load: expect "P|<name>"
		std::ifstream in(defaultFilePath());
		if (!in) {
			throw std::runtime_error("Failed to open file for loading Part");
		}
		std::string kind;
		if (!std::getline(in, kind)) {
			throw std::runtime_error("Invalid file format");
		}
		if (kind.rfind("P|", 0) != 0) {
			throw std::runtime_error("Unexpected record for Part");
		}
		// Name is immutable post-construction in this simple example; loading verifies format.
	}

	// ---------- CompositePart ----------

	CompositePart::CompositePart(std::string name)
		: Part(std::move(name)) {
	}

	void CompositePart::addPart(Part const& p) {
		m_children.emplace_back(p.clone());
	}

	void CompositePart::addPart(std::unique_ptr<Part> p) {
		if (!p) {
			throw std::invalid_argument("addPart requires non-null unique_ptr");
		}
		m_children.emplace_back(std::move(p));
	}

	std::vector<Part*> CompositePart::getParts() const {
		std::vector<Part*> out;
		out.reserve(m_children.size());
		for (auto const& up : m_children) {
			out.push_back(up.get());
		}
		return out;
	}

	bool CompositePart::equalsTo(Part const& other) const {
		// Check base and type
		auto const* oc = dynamic_cast<CompositePart const*>(&other);
		if (!oc) return false;
		if (this->getName() != oc->getName()) return false;
		if (m_children.size() != oc->m_children.size()) return false;
		for (size_t i = 0; i < m_children.size(); ++i) {
			if (!m_children[i]->equalsTo(*oc->m_children[i])) return false;
		}
		return true;
	}

	std::unique_ptr<Part> CompositePart::clone() const {
		auto cp = std::make_unique<CompositePart>(this->getName());
		for (auto const& ch : m_children) {
			cp->addPart(ch->clone());
		}
		return cp;
	}

	void CompositePart::accept(Formatter& formatter, std::ostream& os, int depth) const {
		// Formatter is responsible for traversal policy; we expose "this".
		(void)depth;
		formatter.printParts(*this, os);
	}

	void CompositePart::store() const {
		// Recursive text format:
		// C|<name>
		// {
		//   ... child records ...
		// }
		std::ofstream out(defaultFilePath(), std::ios::trunc);
		if (!out) {
			throw std::runtime_error("Failed to open file for storing CompositePart");
		}
		std::function<void(Part const&, std::ostream&, int)> writeRec;
		writeRec = [&](Part const& p, std::ostream& os, int indent) {
			auto const* comp = dynamic_cast<CompositePart const*>(&p);
			std::string pad(static_cast<size_t>(indent), ' ');
			if (!comp) {
				os << pad << "P|" << p.getName() << "\n";
			} else {
				os << pad << "C|" << comp->getName() << "\n";
				os << pad << "{\n";
				for (auto* child : comp->getParts()) {
					writeRec(*child, os, indent + 2);
				}
				os << pad << "}\n";
			}
		};
		writeRec(*this, out, 0);
	}

	void CompositePart::load() {
		// Minimal parser for the above format; only verifies the root record matches.
		std::ifstream in(defaultFilePath());
		if (!in) {
			throw std::runtime_error("Failed to open file for loading CompositePart");
		}
		std::string first;
		if (!std::getline(in, first)) {
			throw std::runtime_error("Invalid file");
		}
		if (first.rfind("C|", 0) != 0) {
			throw std::runtime_error("Unexpected root record for CompositePart");
		}
		// For brevity of the exercise, we do not rebuild children here.
		// A full implementation would parse the block and reconstruct the tree.
	}

	// ---------- Formatters ----------

	namespace {
		// A leaf is a Part that is not a CompositePart
		inline bool isLeaf(Part const& p) {
			return dynamic_cast<CompositePart const*>(&p) == nullptr;
		}

		void printHierarchy(Part const& p, std::ostream& os, int depth) {
			std::string pad(static_cast<size_t>(depth * 2), ' ');
			os << pad << p.getName() << "\n";
			auto const* comp = dynamic_cast<CompositePart const*>(&p);
			if (comp) {
				for (auto* child : comp->getParts()) {
					printHierarchy(*child, os, depth + 1);
				}
			}
		}
	}

	void HierarchyFormatter::printParts(Part const& p, std::ostream& os) {
		printHierarchy(p, os, 0);
	}

	void SetFormatter::collectLeafCounts(Part const& p,
		std::unordered_map<std::string, int>& counts,
		std::vector<std::string>& insertionOrder) const {
		auto const* comp = dynamic_cast<CompositePart const*>(&p);
		if (!comp) {
			auto it = counts.find(p.getName());
			if (it == counts.end()) {
				counts.emplace(p.getName(), 1);
				insertionOrder.push_back(p.getName());
			} else {
				it->second += 1;
			}
			return;
		}
		for (auto* child : comp->getParts()) {
			collectLeafCounts(*child, counts, insertionOrder);
		}
	}

	void SetFormatter::printParts(Part const& p, std::ostream& os) {
		os << p.getName() << ":\n";
		std::unordered_map<std::string, int> counts;
		std::vector<std::string> order;
		collectLeafCounts(p, counts, order);
		for (auto const& name : order) {
			os << "  " << counts[name] << " " << name << "\n";
		}
	}

	// ---------- Factories ----------

	std::unique_ptr<Part> makePart(std::string name) {
		// Local leaf implementation
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

	std::unique_ptr<CompositePart> makeComposite(std::string name) {
		return std::make_unique<CompositePart>(std::move(name));
	}
}



