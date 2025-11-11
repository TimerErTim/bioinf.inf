#include "pch.h"
#include "partlists.h"

#include <fstream>
#include <cctype>
#include <stdexcept>

using namespace PartLists;

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
	std::string base = getName();
	for (char& ch : base) {
		if (!std::isalnum(static_cast<unsigned char>(ch))) {
			ch = '_';
		}
	}
	return base + ".plist";
}

void Part::store() const {
	std::ofstream out(defaultFilePath(), std::ios::trunc);
	if (!out) {
		throw std::runtime_error("Failed to open file for storing Part");
	}
	out << "P|" << getName() << "\n";
}

void Part::load() {
	std::ifstream in(defaultFilePath());
	if (!in) {
		throw std::runtime_error("Failed to open file for loading Part");
	}
	std::string line;
	if (!std::getline(in, line)) {
		throw std::runtime_error("Invalid file format");
	}
	if (line.rfind("P|", 0) != 0) {
		throw std::runtime_error("Unexpected record for Part");
	}
}


