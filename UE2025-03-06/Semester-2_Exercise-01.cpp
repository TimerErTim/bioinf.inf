// Semester-2_Exercise-01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int* get_pointer() {
    int x(99);
    int* p(&x);
    return p;
}

char* get_string() {
    char* str = new char[5];
    str[0] = 'c';
    str[1] = 'h';
    str[2] = 'a';
    str[3] = 'r';
    str[4] = 0;
    return str;
}

int main()
{
    int* p2 = get_pointer();
    *p2 = 82;
    std::cout << "p2 = " << p2 << std::endl;

    char* new_str = get_string();
    std::cout << "new_str = " << new_str << std::endl;

    char* lets_read = new char[20];
    std::cin.getline(lets_read, 20);
    std::cout << "lets_read = " << lets_read << std::endl;

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
