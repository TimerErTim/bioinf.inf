#include <iostream>

#include "buffer.hpp"

namespace {
    void test_buffer_bad() {
        std::cout << "--- buffer_bad (shallow-copy pitfall) ---\n";
        buffer_bad a(5);
        a[0] = 2;

        buffer_bad b = a;
        std::cout << "&a[0] = " << &a[0] << ", &b[0] = " << &b[0] << "\n";
        
        std::cout << "Expect crash here becuase calling delete on same memory twice \n";
    }

    void test_buffer_good() {
        std::cout << "--- buffer_good (deep-copy) ---\n";
        buffer_good a(5);
        a[0] = 2;

        buffer_good b = a;
        std::cout << "&a[0] = " << &a[0] << ", &b[0] = " << &b[0] << "\n";
        
        buffer_good c = std::move(b);
        std::cout << "&a[0] = " << &a[0] << ", &c[0] = " << &c[0] << "\n";
        
        std::cout << "No crash here because calling delete on same memory twice \n";
    }
}

int main() {
    test_buffer_good();
    test_buffer_bad();
    return 0;
}