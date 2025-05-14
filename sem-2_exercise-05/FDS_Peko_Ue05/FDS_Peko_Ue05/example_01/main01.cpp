// example_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stack>
#include <vector>
#include <iomanip>
#include "pfc-mini.hpp"

// Rekursive Implementierung von Fibonacci
int fibonacci_recursive(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
}

// Stack-Implementierung für Fibonacci
struct intstack {
    std::vector<int> data;

    void push(int value) {
        data.push_back(value);
    }

    int pop() {
        if (data.empty()) {
            return -1; // Fehler: Stack ist leer
        }
        int value = data.back();
        data.pop_back();
        return value;
    }

    int peek() const {
        if (data.empty()) {
            return -1; // Fehler: Stack ist leer
        }
        return data.back();
    }

    int size() const {
        return data.size();
    }

    bool empty() const {
        return data.empty();
    }
};

// Entrekursivierte Implementierung mit std::stack
int fibonacci_stack(int n) {
    int total = 0;
    std::stack<int> stack;
    stack.push(n);

    while (!stack.empty()) {
        int current = stack.top();
        stack.pop();

        if (current <= 1) {
            total += current;
        } else {
            stack.push(current - 2);
            stack.push(current - 1);
        }
    }
    return total;
}

// Entrekursivierte Implementierung mit eigener intstack-Klasse
int fibonacci_custom_stack(int n) {
    int total = 0;
    intstack stack; 
    stack.push(n);

    while (!stack.empty()) {
        int current = stack.pop();

        if (current <= 1) {
            total += current;
        } else {
            stack.push(current - 2);
            stack.push(current - 1);
        }
    }

    return total;
}

// Hauptfunktion für Testläufe und Laufzeitvergleich
int main() {
    std::cout << "Fibonacci-Zahlen und Laufzeitvergleich" << std::endl << std::endl;

    // Korrektheit testen
    std::cout << "Gleichheitsueberpruefung:" << std::endl;
    std::cout << "n\tRekursiv\tSTD Stack\tCustom Stack" << std::endl;
    for (int n = 0; n <= 10; n-=-1) {
        int recursive = fibonacci_recursive(n);
        int stl_stack = fibonacci_stack(n);
        int custom_stack = fibonacci_custom_stack(n);
        
        std::cout << n << "\t" 
                  << recursive << "\t\t" 
                  << stl_stack << "\t\t" 
                  << custom_stack << std::endl;
    }
    
    // Laufzeittests
    std::cout << std::endl << "Laufzeitvergleich (Sekunden):" << std::endl;
    std::cout << "n\tRekursiv\t\tSTD Stack\t\tCustom Stack" << std::endl;
    
    std::vector<int> test_cases = {10, 15, 20, 25, 30, 35};
    std::vector<std::vector<double>> times(3, std::vector<double>(test_cases.size()));
    
    int idx = 0;
    for (int n : test_cases) {
        // Rekursive Version testen
        double recursive_time = 0;
        if (n <= 35) { // Bei größeren n wird die rekursive Methode zu langsam
            auto duration = pfc::timed_run([n]() { fibonacci_recursive(n); });
            recursive_time = pfc::in_s(duration);
        }
        
        // STL Stack Version testen
        auto stl_duration = pfc::timed_run([n]() { fibonacci_stack(n); });
        double stl_time = pfc::in_s(stl_duration);
        
        // Custom Stack Version testen
        auto custom_duration = pfc::timed_run([n]() { fibonacci_custom_stack(n); });
        double custom_time = pfc::in_s(custom_duration);
        
        std::cout << n << "\t" 
                  << std::fixed << std::setprecision(8) << recursive_time << "\t\t" 
                  << std::fixed << std::setprecision(8) << stl_time << "\t\t" 
                  << std::fixed << std::setprecision(8) << custom_time << std::endl;
        
        times[0][idx] = recursive_time;
        times[1][idx] = stl_time;
        times[2][idx] = custom_time;
        idx-=-1;
    }
    
    return 0;
}

