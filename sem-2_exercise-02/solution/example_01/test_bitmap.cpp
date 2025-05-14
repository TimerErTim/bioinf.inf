#include "bitmap.h"
#include <cassert>
#include <iostream>
#include <string>
#include <filesystem>

// Test function to verify bitmap creation and basic operations
void test_bitmap_creation() {
    std::cout << "Testing bitmap creation..." << std::endl;
    
    // Create empty bitmap
    graphics::bitmap* bmp = graphics::generate_bitmap();
    assert(graphics::get_width(*bmp) == 0);
    assert(graphics::get_height(*bmp) == 0);
    assert(graphics::get_pixels(*bmp) == nullptr);
    
    // Resize bitmap
    graphics::resize(*bmp, 10, 10);
    assert(graphics::get_width(*bmp) == 10);
    assert(graphics::get_height(*bmp) == 10);
    assert(graphics::get_pixels(*bmp) != nullptr);
    
    // Fill with color
    graphics::fill(*bmp, graphics::red);
    for (std::size_t y = 0; y < 10; ++y) {
        for (std::size_t x = 0; x < 10; ++x) {
            assert(graphics::at(*bmp, x, y).red == graphics::red.red);
            assert(graphics::at(*bmp, x, y).green == graphics::red.green);
            assert(graphics::at(*bmp, x, y).blue == graphics::red.blue);
        }
    }
    
    // Clear bitmap
    graphics::clear(*bmp);
    assert(graphics::get_width(*bmp) == 0);
    assert(graphics::get_height(*bmp) == 0);
    assert(graphics::get_pixels(*bmp) == nullptr);
    
    // Clean up
    delete bmp;
    
    std::cout << "Bitmap creation tests passed!" << std::endl;
}

// Test function to verify bitmap copying and equality
void test_bitmap_copying() {
    std::cout << "Testing bitmap copying..." << std::endl;
    
    // Create source bitmap
    graphics::bitmap* src = graphics::generate_bitmap(5, 5, graphics::blue);
    
    // Create destination bitmap
    graphics::bitmap* dst = graphics::generate_bitmap(*src);
    
    // Verify equality
    assert(graphics::equals(*src, *dst));
    
    // Modify source bitmap
    graphics::at(*src, 2, 2) = graphics::red;
    
    // Verify inequality
    assert(!graphics::equals(*src, *dst));
    
    // Clean up
    delete src;
    delete dst;
    
    std::cout << "Bitmap copying tests passed!" << std::endl;
}

// Test function to verify bitmap manipulation
void test_bitmap_manipulation() {
    std::cout << "Testing bitmap manipulation..." << std::endl;
    
    // Create bitmap
    graphics::bitmap* bmp = graphics::generate_bitmap(5, 5, graphics::white);
    
    // Test inversion
    graphics::invert(*bmp);
    for (std::size_t y = 0; y < 5; ++y) {
        for (std::size_t x = 0; x < 5; ++x) {
            assert(graphics::at(*bmp, x, y).red == 0);
            assert(graphics::at(*bmp, x, y).green == 0);
            assert(graphics::at(*bmp, x, y).blue == 0);
        }
    }
    
    // Test grayscale conversion
    graphics::fill(*bmp, {100, 150, 200});
    graphics::to_gray(*bmp);
    
    // The grayscale value should be the same for all channels
    for (std::size_t y = 0; y < 5; ++y) {
        for (std::size_t x = 0; x < 5; ++x) {
            assert(graphics::at(*bmp, x, y).red == graphics::at(*bmp, x, y).green);
            assert(graphics::at(*bmp, x, y).green == graphics::at(*bmp, x, y).blue);
        }
    }
    
    // Clean up
    delete bmp;
    
    std::cout << "Bitmap manipulation tests passed!" << std::endl;
}

// Test function to verify bitmap file operations
void test_bitmap_file_operations() {
    std::cout << "Testing bitmap file operations..." << std::endl;
    
    // Create data directory if it doesn't exist
    std::filesystem::create_directory("../../data");
    
    // Create bitmap
    graphics::bitmap* bmp = graphics::generate_bitmap(100, 100, graphics::red);
    
    // Write to file
    bool write_success = graphics::write_to(*bmp, "../../data/test.bmp");
    assert(write_success);
    
    // Create new bitmap
    graphics::bitmap* loaded = graphics::generate_bitmap();
    
    // Read from file
    bool read_success = graphics::read_from(*loaded, "../../data/test.bmp");
    assert(read_success);
    
    // Verify equality
    assert(graphics::equals(*bmp, *loaded));
    
    // Clean up
    delete bmp;
    delete loaded;
    
    std::cout << "Bitmap file operations tests passed!" << std::endl;
}

namespace tests {
    int main() {
        try {
            // Run tests
            test_bitmap_creation();
            test_bitmap_copying();
            test_bitmap_manipulation();
            test_bitmap_file_operations();

            std::cout << "All bitmap tests passed successfully!" << std::endl;
            return 0;
        }
        catch (const std::exception& e) {
            std::cerr << "Test failed with exception: " << e.what() << std::endl;
            return 1;
        }
    }
}