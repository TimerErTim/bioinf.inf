#include "pch.h"
#include "pch.h"
/**
 * @file formatters.cpp
 * @brief Implementations of hierarchy and set formatters for PartLists.
 *
 * - HierarchyFormatter: prints the tree with indentation (2 spaces per level).
 * - SetFormatter: prints a flat multiset of leaf names with occurrence counts.
 */
#include "partlists.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <ostream>

using namespace PartLists;

namespace {
	/**
	 * @brief Determine whether a part is a leaf (i.e., not a CompositePart).
	 * @param p Part to test.
	 * @return true if p is not a CompositePart; false otherwise.
	 */
	inline bool isLeaf(Part const& p) {
		return dynamic_cast<CompositePart const*>(&p) == nullptr;
	}

	/**
	 * @brief Recursively print the hierarchy with indentation.
	 * @param p Current part.
	 * @param os Output stream.
	 * @param depth Current recursion depth (0 for root).
	 */
	void printHierarchyRec(Part const& p, std::ostream& os, int depth) {
		std::string pad(static_cast<size_t>(depth * 2), ' ');
		os << pad << p.getName() << "\n";
		auto const* comp = dynamic_cast<CompositePart const*>(&p);
		if (!comp) return;
		// Pre-order traversal preserves parent-before-children layout.
		for (auto* child : comp->getParts()) {
			printHierarchyRec(*child, os, depth + 1);
		}
	}
}

/**
 * @brief Print the full hierarchy starting at the given part.
 * @param p Root part to print.
 * @param os Destination output stream.
 */
void HierarchyFormatter::printParts(Part const& p, std::ostream& os) {
	printHierarchyRec(p, os, 0);
}

/**
 * @brief Traverse the hierarchy to collect counts of leaf names.
 * @param p Current part or subtree root.
 * @param counts Output map of leaf name to occurrence count.
 * @param order Stable insertion order for first occurrences to keep output deterministic.
 */
void SetFormatter::collectLeafCounts(Part const& p,
	std::unordered_map<std::string, int>& counts,
	std::vector<std::string>& order) const {
	auto const* comp = dynamic_cast<CompositePart const*>(&p);
	if (!comp) {
		auto it = counts.find(p.getName());
		if (it == counts.end()) {
			counts.emplace(p.getName(), 1);
			// Record first-seen order to achieve stable printing across runs.
			order.push_back(p.getName());
		} else {
			it->second += 1;
		}
		return;
	}
	// DFS ensures insertion order corresponds to first encounter in traversal.
	for (auto* child : comp->getParts()) {
		collectLeafCounts(*child, counts, order);
	}
}

/**
 * @brief Print a flat multiset view with counts for leaf parts.
 * @param p Root part to flatten.
 * @param os Destination output stream.
 */
void SetFormatter::printParts(Part const& p, std::ostream& os) {
	os << p.getName() << ":\n";
	std::unordered_map<std::string, int> counts;
	std::vector<std::string> order;
	collectLeafCounts(p, counts, order);
	// Output order is the stable first-seen order collected during traversal.
	for (auto const& name : order) {
		os << "  " << counts[name] << " " << name << "\n";
	}
}


