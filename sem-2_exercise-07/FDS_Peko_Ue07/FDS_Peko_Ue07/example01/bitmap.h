#ifndef BITMAP_H
#define BITMAP_H

#include <cstdint>
#include <string>
#include <iostream>

namespace graphics {
    // Type definitions
    typedef uint_fast8_t byte_type;
    typedef uint_fast32_t uint32_type;
    typedef int_fast32_t long_type;
    typedef uint16_t uint16_type;

    // Pixel structure
    #pragma pack(push, 1)  // Required for byte alignment when doing IO
    struct pixel_type {
        byte_type blue;
        byte_type green;
        byte_type red;
    };
    #pragma pack(pop)

    // File header structure
    #pragma pack(push, 1)
    struct file_header_type {
        union {
            byte_type signature[2];
            uint16_type type;
        };
        uint32_type size;
        uint16_type reserved_1;
        uint16_type reserved_2;
        uint32_type offset;
    };
    #pragma pack(pop)

    // Info header structure
    #pragma pack(push, 1)
    struct info_header_type {
        uint32_type size;
        long_type width;
        long_type height;
        uint16_type planes;
        uint16_type bit_count;
        uint32_type compression;
        uint32_type size_image;
        long_type x_pels_pm;
        long_type y_pels_pm;
        uint32_type clr_used;
        uint32_type clr_important;
    };
    #pragma pack(pop)

    // Predefined colors
    const pixel_type white = { 255, 255, 255 };
    const pixel_type black = { 0, 0, 0 };
    const pixel_type red = { 0, 0, 255 };
    const pixel_type green = { 0, 255, 0 };
    const pixel_type blue = { 255, 0, 0 };

    // Bitmap struct with constructur and destructor
    struct bitmap {
        long_type width;
        long_type height;
        pixel_type* pixels;

        bitmap() : width(0), height(0), pixels(nullptr) {}
        
        ~bitmap() {
            if (pixels != nullptr) {
                delete[] pixels;
                pixels = nullptr;
            }
        }
    };

    // Functions to create bitmap objects
    bitmap* generate_bitmap();
    bitmap* generate_bitmap(std::size_t const w, std::size_t const h, pixel_type const c = white);
    bitmap* generate_bitmap(char const* const p_name);
    bitmap* generate_bitmap(std::string const& name);
    bitmap* generate_bitmap(std::istream& in);
    bitmap* generate_bitmap(bitmap const& src);

    // Functions to initialize bitmap objects
    void clear(bitmap& bmp);
    void resize(bitmap& bmp, std::size_t const w, std::size_t const h, pixel_type const c = white);

    // Function to compare bitmap objects
    bool equals(bitmap const& lhs, bitmap const& rhs);

    // Functions for attribute access
    pixel_type& at(bitmap& bmp, std::size_t const x, std::size_t const y);
    pixel_type const& at(bitmap const& bmp, std::size_t const x, std::size_t const y);
    long_type get_height(bitmap const& bmp);
    long_type get_width(bitmap const& bmp);
    long_type get_image_size(bitmap const& bmp);
    long_type get_num_pixels(bitmap const& bmp);
    byte_type* get_image(bitmap& bmp);
    byte_type const* get_image(bitmap const& bmp);
    pixel_type* get_pixels(bitmap& bmp);
    pixel_type const* get_pixels(bitmap const& bmp);

    // Functions for reading and writing
    bool read_from(bitmap& bmp, char const* const p_name);
    bool read_from(bitmap& bmp, std::string const& name);
    bool read_from(bitmap& bmp, std::istream& in);
    bool write_to(bitmap const& bmp, char const* const p_name);
    bool write_to(bitmap const& bmp, std::string const& name);
    bool write_to(bitmap const& bmp, std::ostream& out);

    // Functions for image manipulation
    void detect_edges(bitmap& bmp);
    void fill(bitmap& bmp, pixel_type const color = white);
    void invert(bitmap& bmp);
    void to_gray(bitmap& bmp);
    int count_and_color_objects(bitmap& src, bitmap& dst, float theta);

    // Other functions
    void swap(bitmap& lhs, bitmap& rhs);
}

#endif