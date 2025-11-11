#include "pch.h"
#include "pch.h"
/**
 * @file composite_part.cpp
 * @brief Implementation of CompositePart composite operations and persistence.
 *
 * Provides deep-copying child insertion, structural equality, cloning,
 * formatter acceptance, and a simple human-readable persistence format.
 */
#include "partlists.h"

#include <fstream>
#include <functional>
#include <stdexcept>

using namespace PartLists;

/**
 * @brief Construct a composite with a validated name.
 * @param name Non-empty name.
 * @throws std::invalid_argument If the name is empty (enforced by base Part).
 */
CompositePart::CompositePart(std::string name)
	: Part(std::move(name)) {
}

/**
 * @brief Add a deep-copied child preserving dynamic type.
 * @param p Part to copy.
 */
void CompositePart::addPart(Part const& p) {
	// Important: use clone() to avoid object slicing and to preserve the
	// exact runtime type of the inserted part.
	m_children.emplace_back(p.clone());
}

/**
 * @brief Add a child by transferring ownership.
 * @param p Unique pointer to child; must be non-null.
 * @throws std::invalid_argument If p is null.
 */
void CompositePart::addPart(std::unique_ptr<Part> p) {
	if (!p) {
		throw std::invalid_argument("addPart requires non-null unique_ptr");
	}
	// Ownership of the child is transferred to this composite; children lifetimes
	// are tied to the composite's lifetime.
	m_children.emplace_back(std::move(p));
}

/**
 * @brief Get non-owning raw pointers to children in insertion order.
 * @return std::vector<Part*> Pointers valid for the lifetime of this composite.
 */
std::vector<Part*> CompositePart::getParts() const {
	std::vector<Part*> out;
	out.reserve(m_children.size());
	for (auto const& up : m_children) {
		out.push_back(up.get());
	}
	return out;
}

/**
 * @brief Structural equality check against another part.
 * @param other Part to compare.
 * @return true if names match and children are pairwise equal in order.
 */
bool CompositePart::equalsTo(Part const& other) const {
	// Note: equality is order-sensitive by design (BOM sequence matters).
	auto const* oc = dynamic_cast<CompositePart const*>(&other);
	if (!oc) return false;
	if (this->getName() != oc->getName()) return false;
	if (m_children.size() != oc->m_children.size()) return false;
	for (size_t i = 0; i < m_children.size(); ++i) {
		if (!m_children[i]->equalsTo(*oc->m_children[i])) return false;
	}
	return true;
}

/**
 * @brief Deep clone this composite and all descendants.
 * @return std::unique_ptr<Part> Newly allocated deep copy.
 */
std::unique_ptr<Part> CompositePart::clone() const {
	// Create a structurally equivalent composite and recursively clone children.
	auto cp = std::make_unique<CompositePart>(this->getName());
	for (auto const& ch : m_children) {
		cp->addPart(ch->clone());
	}
	return cp;
}

/**
 * @brief Accept a formatter to print this composite.
 * @param formatter Formatter to use.
 * @param os Output stream.
 * @param depth Optional indentation depth (ignored here; formatters control layout).
 */
void CompositePart::accept(Formatter& formatter, std::ostream& os, int depth) const {
	(void)depth;
	// Delegation: actual traversal/formatting strategy is decided by the Formatter.
	formatter.printParts(*this, os);
}

/**
 * @brief Store the composite using a simple recursive text format.
 *
 * Format:
 *   C|<name>
 *   {
 *     P|<leaf>
 *     C|<child>
 *     {
 *       ...
 *     }
 *   }
 *
 * @throws std::runtime_error On file open failures.
 */
void CompositePart::store() const {
	std::ofstream out(defaultFilePath(), std::ios::trunc);
	if (!out) {
		throw std::runtime_error("Failed to open file for storing CompositePart");
	}
	std::function<void(Part const&, std::ostream&, int)> writeRec;
	writeRec = [&](Part const& p, std::ostream& os, int indent) {
		auto const* comp = dynamic_cast<CompositePart const*>(&p);
		std::string pad(static_cast<size_t>(indent), ' ');
		if (!comp) {
			// Leaf node: single-line record.
			os << pad << "P|" << p.getName() << "\n";
		} else {
			// Composite node: header + child block.
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

/**
 * @brief Load/validate composite header from default file path.
 * @details This simplified loader verifies the root record. A full parser could
 *          reconstruct children (out of scope for this exercise).
 * @throws std::runtime_error On I/O or format errors.
 */
void CompositePart::load() {
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
	// Full reconstruction parser could be implemented here if required.
}


