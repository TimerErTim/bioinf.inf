#pragma once

#include <vector>
#include <iostream>
#include <string>

class heap_sorter {
public:
	using content_t = std::vector<int>;
	using index_t = content_t::size_type;
	using size_t = content_t::size_type;
	using value_t = content_t::value_type;

	static void sort(content_t& c);
	static void print_content(const content_t& c);
	static void print_as_tree(const content_t& c);

private:
	static void build_heap(content_t& c);
	static void shift_down(content_t& c, index_t start, size_t len);
	static void print_as_tree(
		const content_t& c,
		const index_t i,
		const size_t len,
		const size_t depth,
		std::ostream& out = std::cout
	);
};
