#include "heapsort.hpp"

void heap_sorter::sort(content_t& c)
{
}

void heap_sorter::print_content(const content_t& c)
{
}

void heap_sorter::print_as_tree(const content_t& c)
{
}

using index_t = heap_sorter::index_t;
static constexpr index_t left(index_t i) {
	return (i * 2) + 1;
}
static constexpr index_t right(index_t i) {
	return (i * 2) + 2;
}
static constexpr index_t parent(index_t i) {
	return (i - 1) / 2;
}

void heap_sorter::print_as_tree(const content_t& c, const index_t i, const size_t depth, std::ostream& out)
{
}

void heap_sorter::build_heap(content_t& c)
{
}

void heap_sorter::shift_down(content_t& c, index_t start, size_t len)
{

}

