// example_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "./bitmap.h"
#include "./test_bitmap.h"
#include <cassert>
#include <iostream>
#include <filesystem>

namespace example {
    int main() {
        // Create a data directory if it doesn't exist
        std::filesystem::create_directory("../../data");

        // Modified example from the sheet
        graphics::bitmap* b0 = graphics::generate_bitmap(300, 100);

        graphics::bitmap* b1 = graphics::generate_bitmap(300, 100, graphics::red);
        
        delete b0;
        b0 = b1;
        assert(graphics::equals(*b0, *b1));


        graphics::bitmap* b2 = graphics::generate_bitmap("../../erythrocytes.bmp");
        graphics::bitmap* b3 = graphics::generate_bitmap(*b2);
        graphics::detect_edges(*b3);
        graphics::invert(*b2);

        assert(graphics::write_to(*b1, "../../data/output-1.bmp"));
        assert(graphics::write_to(*b2, "../../data/output-2.bmp"));
        assert(graphics::write_to(*b3, "../../data/output-3.bmp"));
        delete b3;
        delete b2;
        delete b1;

        std::cout << "Bitmap operations completed successfully!" << std::endl;
        return 0;
    }
}

int main() {
    return example::main();
}

