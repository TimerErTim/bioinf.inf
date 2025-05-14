#include <iostream>
#include <cmath>

// Function pointer type for the function to be analyzed
using func_ptr = double (*)(double);

double bisection(func_ptr f, double a, double b, double epsilon = 1e-4) {
    // Calculate function values at interval boundaries
    double fa = f(a);
    double fb = f(b);
    
    // Check prerequisites
    if (fa * fb >= 0) {
        std::cout << "Error: Function has same signs at interval bounds" << std::endl;
        return 0.0;
    }
    
    // Calculate interval midpoint
    double c = (a + b) / 2.0;
    double fc = f(c);
    
    // Check if interval width is small enough
    if (std::abs(b - a) < epsilon) {
        return c;
    }
    
    // Recursive call for the corresponding interval half
    if (fa * fc < 0) {
        return bisection(f, a, c, epsilon);
    } else {
        return bisection(f, c, b, epsilon);
    }
}

// Test functions
double f1(double x) {
    return 2 * x * x * x - 3 * x * x + 0.5; // Roots at x = -4, 1, 2
}

double f2(double x) {
    return x + exp(x);
}

double f3(double x) {
    return 2.5 * x * x - 7 * x + 4;
}

int main() {
    // Print function definitions
    std::cout << "Function definitions:" << std::endl;
    std::cout << "f1(x) = 2x^3 - 3x^2 + 0.5" << std::endl;
    std::cout << "f2(x) = x + e^x" << std::endl;
    std::cout << "f3(x) = 2.5x^2 - 7x + 4" << std::endl << std::endl;

    // Test f1 with different intervals
    std::cout << "bisection(f1, -2, 0) = " << bisection(f1, -2, 0) << std::endl;
    std::cout << "bisection(f1, -2, 4) = " << bisection(f1, -2, 4) << std::endl << std::endl;

    // Test f2 with different intervals  
    std::cout << "bisection(f2, -1, 1) = " << bisection(f2, -1, 1) << std::endl;
    std::cout << "bisection(f2, 0, 1) = " << bisection(f2, 0, 1) << std::endl << std::endl;

    // Test f3 with different intervals
    std::cout << "bisection(f3, -1, 4) = " << bisection(f3, -1, 4) << std::endl;
    std::cout << "bisection(f3, 1, 4) = " << bisection(f3, 1, 4) << std::endl;
    std::cout << "bisection(f3, -1, 1) = " << bisection(f3, -1, 1) << std::endl << std::endl;

    return 0;
} 