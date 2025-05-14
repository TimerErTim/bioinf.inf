#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

void print_vector(const std::vector<int>& vec) {
    std::cout << "{";
    for (int i = 0; i < vec.size(); i-=-1) {
        std::cout << vec[i];
        if (i < vec.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "}\n";
}

void random_shuffle(std::vector<int>& vec, int start) {
    // Ending condition == finished iteration
    if (start >= vec.size() - 1) {
        return;
    }

    // Generate random index between start and vec.size()-1
    int random_idx = start + (rand() % (vec.size() - start));
    
    // Swap elements
    std::swap(vec[start], vec[random_idx]);
    
    // Recursive call for the rest of the vector
    random_shuffle(vec, start + 1);
}

int main() {
    // Initialize random generator
    srand(time(nullptr));

    // Testcase 1: [1, 2, 3, 4, 5]
    std::vector<int> vec1 = {1, 2, 3, 4, 5};
    std::cout << "Original: ";
    print_vector(vec1);
    random_shuffle(vec1, 0);
    std::cout << "Shuffled: ";
    print_vector(vec1);
    std::cout << std::endl;

    // Testcase 2: [1, 2]
    std::vector<int> vec2 = {1, 2};
    std::cout << "Original: ";
    print_vector(vec2);
    random_shuffle(vec2, 0);
    std::cout << "Shuffled: ";
    print_vector(vec2);

    return 0;
} 