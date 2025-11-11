#include "pch.h"
#include "partlists.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <ostream>

using namespace PartLists;

namespace {
	inline bool isLeaf(Part const& p) {
		return dynamic_cast<CompositePart const*>(&p) == nullptr;
	}

	void printHierarchyRec(Part const& p, std::ostream& os, int depth) {
		std::string pad(static_cast<size_t>(depth * 2), ' ');
		os << pad << p.getName() << "\n";
		auto const* comp = dynamic_cast<CompositePart const*>(&p);
		if (!comp) return;
		for (auto* child : comp->getParts()) {
			printHierarchyRec(*child, os, depth + 1);
		}
	}
}

void HierarchyFormatter::printParts(Part const& p, std::ostream& os) {
	printHierarchyRec(p, os, 0);
}

void SetFormatter::collectLeafCounts(Part const& p,
	std::unordered_map<std::string, int>& counts,
	std::vector<std::string>& order) const {
	auto const* comp = dynamic_cast<CompositePart const*>(&p);
	if (!comp) {
		auto it = counts.find(p.getName());
		if (it == counts.end()) {
			counts.emplace(p.getName(), 1);
			order.push_back(p.getName());
		} else {
			it->second += 1;
		}
		return;
	}
	for (auto* child : comp->getParts()) {
		collectLeafCounts(*child, counts, order);
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


