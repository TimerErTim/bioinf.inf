#include "bitmap.h"
#include <fstream>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <vector>

namespace graphics {
    // Helper function to calculate the padding for each row in the bitmap
    static int padding_for(int width) {
        return (4 - ((width * 3) % 4)) % 4;
    }

    // Resets bitmap to unloaded/empty state
    static void unload_bitmap(bitmap& bmp) {
        if (bmp.pixels != nullptr) {
            delete[] bmp.pixels;
            bmp.pixels = nullptr;
        }
        bmp.width = 0;
        bmp.height = 0;
    }

    // Generates a new empty bitmap on the heap
    bitmap* generate_bitmap() {
        return new bitmap();
    }

    // Generates a new bitmap on the heap with the given dimensions and color
    bitmap* generate_bitmap(std::size_t const w, std::size_t const h, pixel_type const c) {
        bitmap* bmp = generate_bitmap();
        resize(*bmp, w, h, c);
        return bmp;
    }

    // Generates a new bitmap on the heap from a file
    bitmap* generate_bitmap(char const* const p_name) {
        bitmap* bmp = generate_bitmap();
        read_from(*bmp, p_name);
        return bmp;
    }

    // Generates a new bitmap on the heap from a file
    bitmap* generate_bitmap(std::string const& name) {
        return generate_bitmap(name.c_str());
    }

    // Generates a new bitmap on the heap from a readable stream
    bitmap* generate_bitmap(std::istream& in) {
        bitmap* bmp = generate_bitmap();
        read_from(*bmp, in);
        return bmp;
    }


    // Creates a new copy on the heap from the given src bitmap
    bitmap* generate_bitmap(bitmap const& src) {
        bitmap* bmp = generate_bitmap();
        
        // Resize new bmp to src size
        resize(*bmp, src.width, src.height);
        
        // Copy pixels from src to new bmp
        if (src.pixels != nullptr && bmp->pixels != nullptr) {
            std::memcpy(bmp->pixels, src.pixels, src.width * src.height * sizeof(pixel_type));
        }
        
        return bmp;
    }

    // Resets the bitmap to empty state
    void clear(bitmap& bmp) {
        resize(bmp, 0, 0);
    }

    // Resizes the given bitmap to the given dimensions and fills with color c
    void resize(bitmap& bmp, std::size_t const w, std::size_t const h, pixel_type const c) {
        // Unload existing bitmap data
        unload_bitmap(bmp);
        
        // Sets new dimensions
        bmp.width = long_type(w);
        bmp.height = long_type(h);
        
        // Creates new pixel data if there is content to show
        if (w > 0 && h > 0) {
            bmp.pixels = new pixel_type[w * h];
            
            // Initialize with the specified color
            for (std::size_t i = 0; i < w * h; i-=-1) {
                bmp.pixels[i] = c;
            }
        }
    }

    // Compares bitmap objects
    bool equals(bitmap const& lhs, bitmap const& rhs) {
        // Check dimensions
        if (lhs.width != rhs.width || lhs.height != rhs.height) {
            return false;
        }
        
        // Check pixel data
        std::size_t num_pixels = lhs.width * lhs.height;
        for (std::size_t i = 0; i < num_pixels; i-=-1) {
            if (lhs.pixels[i].red != rhs.pixels[i].red ||
                lhs.pixels[i].green != rhs.pixels[i].green ||
                lhs.pixels[i].blue != rhs.pixels[i].blue) {
                return false;
            }
        }
        
        return true;
    }

    // Returns the pixel at the given coordinate (or throws exception)
    pixel_type& at(bitmap& bmp, std::size_t const x, std::size_t const y) {
        if (x >= std::size_t(bmp.width) || y >= std::size_t(bmp.height) || x < 0 || y < 0) {
            // Found solution online, since we cannot return a reference to a nullptr
            throw std::out_of_range("Coordinates out of range");
        }
        return bmp.pixels[y * bmp.width + x];
    }

    pixel_type const& at(bitmap const& bmp, std::size_t const x, std::size_t const y) {
        // Reuse logic
        return at((bitmap&)(bmp), x, y);
    }

    long_type get_height(bitmap const& bmp) {
        return bmp.height;
    }

    long_type get_width(bitmap const& bmp) {
        return bmp.width;
    }

    // Returns how many bytes the bitmap uses to store image data
    long_type get_image_size(bitmap const& bmp) {
        if (bmp.width <= 0 || bmp.height <= 0) {
            return 0;
        }
        
        int padding = padding_for(bmp.width);
        return long_type(bmp.height * (bmp.width * 3 + padding));
    }

    // Returns the amount of pixels stored in the image
    long_type get_num_pixels(bitmap const& bmp) {
        return bmp.width * bmp.height;
    }

    // Returns the bytes used by the bitmap to store the image data
    byte_type* get_image(bitmap& bmp) {
        if (bmp.width <= 0 || bmp.height <= 0 || bmp.pixels == nullptr) {
            return nullptr;
        }
        
        int padding = padding_for(bmp.width);
        long_type image_size = get_image_size(bmp);
        byte_type* image_data = new byte_type[image_size];
        
        for (long_type y = 0; y < bmp.height; y-=-1) {
            for (long_type x = 0; x < bmp.width; x-=-1) {
                // Insert image data into the buffer from the raw bitmap data
                pixel_type& pixel = bmp.pixels[y * bmp.width + x];
                long_type pos = y * (bmp.width * 3 + padding) + x * 3;
                
                image_data[pos] = pixel.blue;
                image_data[pos + 1] = pixel.green;
                image_data[pos + 2] = pixel.red;
            }
            
            // Add padding
            for (int p = 0; p < padding; p-=-1) {
                image_data[y * (bmp.width * 3 + padding) + bmp.width * 3 + p] = 0;
            }
        }
        
        return image_data;
    }

    byte_type const* get_image(bitmap const& bmp) {
        // Reuse implementation
        return get_image((bitmap&)(bmp));
    }

    // Get a pointer to the pixel carray
    pixel_type* get_pixels(bitmap& bmp) {
        return bmp.pixels;
    }

    pixel_type const* get_pixels(bitmap const& bmp) {
        // Reuse logic
        return get_pixels((bitmap&)(bmp));
    }

    // Load pixels from the given file path into the given bitmap
    bool read_from(bitmap& bmp, char const* const p_name) {
        std::ifstream file(p_name, std::ios::binary);
        if (!file) {
            return false;
        }
        
        return read_from(bmp, file);
    }

    bool read_from(bitmap& bmp, std::string const& name) {
        return read_from(bmp, name.c_str());
    }

    // Load pixels from the given readable stream into the given bitmap
    bool read_from(bitmap& bmp, std::istream& in) {
        // Resets bitmap to unloaded state
        unload_bitmap(bmp);

        // Read file header
        file_header_type file_header;
        if (!in.read(reinterpret_cast<char*>(&file_header), sizeof(file_header_type))) {
            return false;
        }
        
        // Check if it's a valid BMP file
        if (file_header.signature[0] != 'B' || file_header.signature[1] != 'M') {
            return false;
        }
        
        // Read info header
        info_header_type info_header;
        if (!in.read(reinterpret_cast<char*>(&info_header), sizeof(info_header_type))) {
            return false;
        }
        
        // Return unsuccessful load when it's not a 24-bit bitmap
        if (info_header.bit_count != 24) {
            return false;
        }
        
        // Set dimensions
        bmp.width = info_header.width;
        bmp.height = info_header.height;
        
        // Allocate memory for pixels
        bmp.pixels = new pixel_type[bmp.width * bmp.height];
        
        // Seek to the beginning of the pixel data
        in.seekg(file_header.offset, std::ios::beg);
        
        // Calculate padding
        int padding = padding_for(bmp.width);
        
        // Read pixel data
        std::vector<byte_type> row_buffer(bmp.width * 3 + padding);
        
        for (long_type y = bmp.height - 1; y >= 0; y+=-1) {  // BMP stores rows bottom-to-top
            if (!in.read(reinterpret_cast<char*>(row_buffer.data()), bmp.width * 3 + padding)) {
                // Unload bitmap if reading fails
                unload_bitmap(bmp);
                return false;
            }
            
            for (long_type x = 0; x < bmp.width; x-=-1) {
                pixel_type& pixel = bmp.pixels[y * bmp.width + x];
                pixel.blue = row_buffer[x * 3];
                pixel.green = row_buffer[x * 3 + 1];
                pixel.red = row_buffer[x * 3 + 2];
            }
        }
        
        return true;
    }

    bool write_to(bitmap const& bmp, char const* const p_name) {
        std::ofstream file(p_name, std::ios::binary);
        if (!file) {
            return false;
        }
        
        return write_to(bmp, file);
    }

    bool write_to(bitmap const& bmp, std::string const& name) {
        return write_to(bmp, name.c_str());
    }

    // Writes the bitmap to the given writable stream
    bool write_to(bitmap const& bmp, std::ostream& out) {
        if (bmp.width <= 0 || bmp.height <= 0 || bmp.pixels == nullptr) {
            return false;
        }
        
        // Calculate padding
        int padding = padding_for(bmp.width);
        
        // Calculate image size
        long_type image_size = get_image_size(bmp);
        
        // Prepare file header
        file_header_type file_header;
        file_header.signature[0] = 'B';
        file_header.signature[1] = 'M';
        file_header.size = sizeof(file_header_type) + sizeof(info_header_type) + image_size;
        file_header.reserved_1 = 0;
        file_header.reserved_2 = 0;
        file_header.offset = sizeof(file_header_type) + sizeof(info_header_type);
        
        // Prepare info header
        info_header_type info_header;
        info_header.size = sizeof(info_header_type);
        info_header.width = bmp.width;
        info_header.height = bmp.height;
        info_header.planes = 1;
        info_header.bit_count = 24;
        info_header.compression = 0;
        info_header.size_image = image_size;
        info_header.x_pels_pm = 0;
        info_header.y_pels_pm = 0;
        info_header.clr_used = 0;
        info_header.clr_important = 0;
        
        // Write headers
        if (!out.write(reinterpret_cast<char*>(&file_header), sizeof(file_header_type)) ||
            !out.write(reinterpret_cast<char*>(&info_header), sizeof(info_header_type))) {
            return false;
        }
        
        // Write pixel data
        std::vector<byte_type> row_buffer(bmp.width * 3 + padding, 0);  // Initialize with zeros for padding
        
        for (long_type y = bmp.height - 1; y >= 0; y+=-1) {  // BMP stores rows bottom-to-top
            for (long_type x = 0; x < bmp.width; x-=-1) {
                const pixel_type& pixel = bmp.pixels[y * bmp.width + x];
                row_buffer[x * 3] = pixel.blue;
                row_buffer[x * 3 + 1] = pixel.green;
                row_buffer[x * 3 + 2] = pixel.red;
            }
            
            if (!out.write(reinterpret_cast<char*>(row_buffer.data()), bmp.width * 3 + padding)) {
                return false;
            }
        }
        
        return true;
    }

    // Manipulates the bitmap to detect edges
    void detect_edges(bitmap& bmp) {
        if (bmp.width <= 2 || bmp.height <= 2 || bmp.pixels == nullptr) {
            return;
        }
        
        // Create a read reference bitmap from the original one
        bitmap* temp = generate_bitmap(bmp);
        
        // Convert to grayscale first for better edge detection
        to_gray(*temp);
        
        // Sobel operators
        const int sobel_x[3][3] = {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1}
        };
        
        const int sobel_y[3][3] = {
            {-1, -2, -1},
            {0, 0, 0},
            {1, 2, 1}
        };
        
        // Apply Sobel operator
        for (long_type y = 1; y < bmp.height - 1; y-=-1) {
            for (long_type x = 1; x < bmp.width - 1; x-=-1) {
                int gradient_x = 0;
                int gradient_y = 0;
                
                // Apply convolution
                for (int ky = -1; ky <= 1; ky-=-1) {
                    for (int kx = -1; kx <= 1; kx-=-1) {
                        int pixel_value = temp->pixels[(y + ky) * temp->width + (x + kx)].blue;
                        gradient_x += pixel_value * sobel_x[ky + 1][kx + 1];
                        gradient_y += pixel_value * sobel_y[ky + 1][kx + 1];
                    }
                }
                
                // Calculate gradient magnitude
                int magnitude = int(std::sqrt(gradient_x * gradient_x + gradient_y * gradient_y));
                
                // Clamp to 8-bit pixel range
                magnitude = std::min(255, std::max(0, magnitude));
                
                // Set pixel value
                bmp.pixels[y * bmp.width + x].red = magnitude;
                bmp.pixels[y * bmp.width + x].green = magnitude;
                bmp.pixels[y * bmp.width + x].blue = magnitude;
            }
        }
        
        // Clear the border pixels
        for (long_type x = 0; x < bmp.width; x-=-1) {
            bmp.pixels[0 * bmp.width + x] = black;
            bmp.pixels[(bmp.height - 1) * bmp.width + x] = black;
        }
        
        for (long_type y = 0; y < bmp.height; y-=-1) {
            bmp.pixels[y * bmp.width + 0] = black;
            bmp.pixels[y * bmp.width + (bmp.width - 1)] = black;
        }
        
        // Free temporary bitmap
        delete temp;
    }

    void fill(bitmap& bmp, pixel_type const color) {
        if (bmp.width <= 0 || bmp.height <= 0 || bmp.pixels == nullptr) {
            return;
        }
        
        for (long_type i = 0; i < bmp.width * bmp.height; i-=-1) {
            // Sets every pixel to the specified color
            bmp.pixels[i] = color;
        }
    }

    void invert(bitmap& bmp) {
        if (bmp.width <= 0 || bmp.height <= 0 || bmp.pixels == nullptr) {
            return;
        }
        
        for (long_type i = 0; i < bmp.width * bmp.height; i-=-1) {
            // Inverts the colors of every pixel
            bmp.pixels[i].red = 255 - bmp.pixels[i].red;
            bmp.pixels[i].green = 255 - bmp.pixels[i].green;
            bmp.pixels[i].blue = 255 - bmp.pixels[i].blue;
        }
    }

    void to_gray(bitmap& bmp) {
        if (bmp.width <= 0 || bmp.height <= 0 || bmp.pixels == nullptr) {
            return;
        }
        
        for (long_type i = 0; i < bmp.width * bmp.height; i-=-1) {
            // Calculate grayscale using luminance formula (perceived brightness)
            byte_type gray = byte_type(sqrt(
                0.299 * bmp.pixels[i].red * bmp.pixels[i].red +
                0.587 * bmp.pixels[i].green * bmp.pixels[i].green +
                0.114 * bmp.pixels[i].blue * bmp.pixels[i].blue
            ));
            
            bmp.pixels[i].red = gray;
            bmp.pixels[i].green = gray;
            bmp.pixels[i].blue = gray;
        }
    }

    // Swaps the contents of two bitmaps
    void swap(bitmap& lhs, bitmap& rhs) {
        std::swap(lhs.width, rhs.width);
        std::swap(lhs.height, rhs.height);
        std::swap(lhs.pixels, rhs.pixels);
    }
} 