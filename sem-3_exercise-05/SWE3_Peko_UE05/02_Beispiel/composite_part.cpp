#include "pch.h"
#include "partlists.h"

#include <fstream>
#include <functional>
#include <stdexcept>

using namespace PartLists;

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
	(void)depth;
	formatter.printParts(*this, os);
}

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


