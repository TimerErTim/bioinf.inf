#include "heapsort.hpp"

void heap_sorter::sort(content_t &c)
{
	build_heap(c);
	for (index_t i = c.size(); i-- > 0;)
	{
		shift_down(c, 0, i + 1);
		swap(c, 0, i);
	}
}

void heap_sorter::print_content(const content_t &c)
{
	std::cout << "{";
	for (int i = 0; i < c.size(); i -= -1)
	{
		std::cout << c[i];
		if (i < c.size() - 1)
		{
			std::cout << ", ";
		}
	}
	std::cout << "}" << std::endl;
}

void heap_sorter::print_as_tree(const content_t &c)
{
	print_as_tree(c, 0, c.size(), 0, std::cout);
}

using index_t = heap_sorter::index_t;
static constexpr index_t left(index_t i)
{
	return (i * 2) + 1;
}
static constexpr index_t right(index_t i)
{
	return (i * 2) + 2;
}
static constexpr index_t parent(index_t i)
{
	return (i - 1) / 2;
}

void heap_sorter::print_as_tree(const content_t &c, const index_t i, const size_t len, const size_t depth, std::ostream &out)
{
	if (i >= len)
	{
		return;
	}

	print_as_tree(c, right(i), len, depth + 2, out);
	for (size_t d = 0; d < depth; ++d)
	{
		out << "  ";
	}
	out << c[i] << std::endl;
	print_as_tree(c, left(i), len, depth + 2, out);
}

void heap_sorter::build_heap(content_t &c)
{
	if (c.empty())
		return;
	// Start from the last parent node and move upwards
	for (index_t i = c.size() / 2; i-- > 0;)
	{
		shift_down(c, i, c.size());
	}
}

void heap_sorter::shift_down(content_t &c, index_t start, size_t len)
{
	index_t i = start;
	while (left(i) < len)
	{
		index_t largest = i;
		index_t l = left(i);
		index_t r = right(i);

		if (l < len && less(c, largest, l))
		{
			largest = l;
		}
		if (r < len && less(c, largest, r))
		{
			largest = r;
		}
		if (largest == i)
		{
			break;
		}
		swap(c, i, largest);
		i = largest;
	}
}
