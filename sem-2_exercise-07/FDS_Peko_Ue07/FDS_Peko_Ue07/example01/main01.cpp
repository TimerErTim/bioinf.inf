#include "pfc-mini.hpp"
#include "bitmap.h"
#include <iostream>
#include <string>
#include <vector>

//Function to run a single test case
void run_test_case(int case_num, const std::string& input_path, float theta, bool create_empty = false) {
	std::cout << "Testcase " << case_num << std::endl;
	std::cout << "--------------------------------" << std::endl;

	
	graphics::bitmap bmp_in;
	graphics::bitmap bmp_out;
	int object_count = 0;

	if (input_path.empty()) {
		std::cout << "Input:" << std::endl;
		std::cout << "- bmp_in = empty()" << std::endl;
		// No need to load, bmp_in is already empty
	} else {
		std::cout << "Input:" << std::endl;
		std::cout << "- bmp_in = " << input_path << std::endl;
		if (!graphics::read_from(bmp_in, input_path)) {
			std::cerr << "Error reading input file: " << input_path << std::endl;
			return;
		}
	}
	std::cout << "- theta = " << theta << std::endl;


	object_count = graphics::count_and_color_objects(bmp_in, bmp_out, theta);
	std::cout << "Output:" << std::endl;
	std::cout << "- count = " << object_count << std::endl;

	if (graphics::get_width(bmp_out) > 0 && graphics::get_height(bmp_out) > 0)
	{
		std::string output_filename = "../../assets/testcase-" + std::to_string(case_num) + "_output.bmp";
		if (!graphics::write_to(bmp_out, output_filename))
		{
			std::cerr << "Error writing output file: " << output_filename << std::endl;
			return;
		}
		std::cout << "- bmp_out = " << output_filename << std::endl;
	}
	else
	{
		std::cout << "- bmp_out = empty()" << std::endl;
	}

	std::cout << std::endl << std::endl;
}


int main() {
	run_test_case(1, "", 0.2f, true);

	run_test_case(2, "../../assets/black_picture.bmp", 0.1f);

	run_test_case(3, "../../assets/white_picture.bmp", 0.5f);

	run_test_case(4, "../../assets/3_dots_picture.bmp", 0.5f);

	run_test_case(5, "../../assets/circle_hole_picture.bmp", 0.5f);

	run_test_case(6, "../../assets/white_picture.bmp", 1.1f);

	return 0;
}