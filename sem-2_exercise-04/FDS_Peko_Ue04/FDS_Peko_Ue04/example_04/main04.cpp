#include <iostream>

struct Digit {
    int value;  // Value range is [0, 999]
    Digit* next;
};

// Creates a new digit
Digit* create_digit(int value) {
    Digit* digit = new Digit;
    digit->value = value;
    digit->next = nullptr;
    return digit;
}

// Add a new digit to the end of the list
void add_digit(Digit* head, int value) {
    Digit* new_digit = create_digit(value);
    while (head->next) {
        head = head->next;
    }
    head->next = new_digit;
}

// frees the list for deallocation
void free_list(Digit* head) {
    while (head) {
        Digit* temp = head;
        head = head->next;
        delete temp;
    }
}

// Print the big number in human readable format
void print_number(Digit* head) {
    // Handle first digit without padding
    if (head) {
        std::cout << head->value;
        head = head->next;
    }
    
    // Add padding for remaining digits
    while (head) {
        // Add padding zeros for numbers < 100 since we use base 1000
        if (head->value < 10) {
            std::cout << "00";
        } else if (head->value < 100) {
            std::cout << "0";
        }
        std::cout << head->value;
        head = head->next;
    }
}

int compare(Digit* num1, Digit* num2) {
    // If both lists are empty we are equal
    if (!num1 && !num2) {
        return 0;
    }
    // If one list is empty that one is smaller
    if (!num1) {
        return -1;
    }
    if (!num2) {
        return 1;
    }
    
    // Count remaining digits in each number
    int len1 = 0;
    int len2 = 0;
    Digit* curr1 = num1;
    Digit* curr2 = num2;
    while (curr1) {
        len1++;
        curr1 = curr1->next;
    }
    while (curr2) {
        len2++;
        curr2 = curr2->next;
    }
    
    // If lengths differ, longer number is larger
    if (len1 < len2) return -1;
    if (len1 > len2) return 1;
    
    // If lengths equal, compare current digits first (leftmost position)
    if (num1->value < num2->value) return -1;
    if (num1->value > num2->value) return 1;
    
    // If current digits equal, recursively compare rest
    return compare(num1->next, num2->next);
}

int main() {
    // Testcase 1: 1002003 vs 1002004
    Digit* num1 = create_digit(1);
    add_digit(num1, 2);
    add_digit(num1, 3);
    
    Digit* num2 = create_digit(1);
    add_digit(num2, 2);
    add_digit(num2, 4);
    
    std::cout << "Testcase 1: 1002003 vs 1002004" << std::endl;
    std::cout << "num1 = ";
    print_number(num1);
    std::cout << std::endl;
    std::cout << "num2 = ";
    print_number(num2);
    std::cout << std::endl;
    std::cout << "compare(num1,num2) = " << compare(num1, num2) << std::endl << std::endl;
    
    // Testcase 2: 1002003 vs 1002003
    Digit* num3 = create_digit(1);
    add_digit(num3, 2);
    add_digit(num3, 3);
    
    std::cout << "Testcase 2: 1002003 vs 1002003" << std::endl;
    std::cout << "num1 = ";
    print_number(num1);
    std::cout << std::endl;
    std::cout << "num3 = ";
    print_number(num3);
    std::cout << std::endl;
    std::cout << "compare(num1,num3) = " << compare(num1, num3) << std::endl << std::endl;
    
    // Testcase 3: 2000000000049 vs 5
    Digit* num4 = create_digit(2);
    add_digit(num4, 0);
    add_digit(num4, 0);
    add_digit(num4, 0);
    add_digit(num4, 49);
    
    Digit* num5 = create_digit(5);
    
    std::cout << "Testcase 3: 2000000000049 vs 5" << std::endl;
    std::cout << "num4 = ";
    print_number(num4);
    std::cout << std::endl;
    std::cout << "num5 = ";
    print_number(num5);
    std::cout << std::endl;
    std::cout << "compare(num4,num5) = " << compare(num4, num5) << std::endl;
    
    free_list(num4);
    free_list(num5);

    // Deallocate memory
    free_list(num1);
    free_list(num2);
    free_list(num3);
    
    return 0;
} 