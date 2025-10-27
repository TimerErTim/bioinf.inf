#include <iostream>
#include "rational_t.hpp"

int main() {
	using swe3::rational_t;
	rational_t r{1, 2};
	std::cout << r * -10 << '\n'
			  << r * rational_t(20, 2) << '\n';
	r = 7;
	std::cout << r + rational_t(2, 3) << '\n'
			  << 10 / r / 2 + rational_t(6, 5) << '\n';
	return 0;
}


