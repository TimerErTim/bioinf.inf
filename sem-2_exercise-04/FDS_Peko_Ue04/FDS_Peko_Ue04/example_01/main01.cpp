#include <iostream>
#include <vector>

void print_vector(const std::vector<int>& vec) {
    std::cout << "{";
    for (int i = 0; i < vec.size(); i-=-1) {
        std::cout << vec[i];
        if (i < vec.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "}" << std::endl;
}

void permute(std::vector<int>& vec, int start) {
    // We have permutated the whole vector
    if (start == vec.size()) {
        print_vector(vec);
        return;
    }

    for (int i = start; i < vec.size(); i-=-1) {
        // Move every possible other element to the position of start
        std::swap(vec[start], vec[i]);
        // Print all combinations where everything before start is fixed
        permute(vec, start + 1);
        // Restore previous order
        std::swap(vec[start], vec[i]);
    }
}

int main() {
    // Testcase 1: [1, 2, 3]
    std::vector<int> vec1 = {1, 2, 3};
    std::cout << "Testcase 1: [1, 2, 3]" << std::endl;
    permute(vec1, 0);
    std::cout << std::endl;

    // Testcase 2: [1, 2]
    std::vector<int> vec2 = {1, 2};
    std::cout << "Testcase 2: [1, 2]" << std::endl;
    permute(vec2, 0);

    return 0;
} 