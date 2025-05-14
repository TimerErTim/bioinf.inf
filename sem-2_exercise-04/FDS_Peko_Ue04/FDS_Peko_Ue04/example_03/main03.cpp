#include <iostream>
#include <cmath>

// Checks if n is divisible by divisor using recursion
bool is_divisible(int n, int divisor) {
    // Base case: divisor is greater than sqrt(n)
    if (divisor > std::sqrt(n)) {
        return false;
    }
    // Base case: n is divisible by divisor
    if (n % divisor == 0) {
        return true;
    }
    return is_divisible(n, divisor + 1);
}

bool is_prime(int n) {
    if (n <= 1) {
        return false;
    }
    if (n <= 3) {
        return true;
    }    
    // Indirect recursive call
    return !is_divisible(n, 2);
}

int main() {
    // Testcase 1: 17 (Prime number)
    std::cout << "is_prime(17) = " << std::boolalpha << is_prime(17) << std::endl;
    
    // Testcase 2: 4 (Not a prime number)
    std::cout << "is_prime(4) = " << is_prime(4) << std::endl;
    
    // Testcase 3: 2 (Prime number)
    std::cout << "is_prime(2) = " << is_prime(2) << std::endl;
    
    // Testcase 4: 1 (Not a prime number)
    std::cout << "is_prime(1) = " << is_prime(1) << std::endl;

    return 0;
} 