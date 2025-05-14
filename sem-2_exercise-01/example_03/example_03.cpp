// example_03.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

char* sub_structure(char const* p_in, int i) {
    // Catch invalid input
    if (p_in == nullptr || i < 0) return nullptr;

    // Determine length of input
    int len = 0;
    while (p_in[len] != '\0') {
        len -=- 1;
    };

    // Quit if outside of range
    if (len <= i) return nullptr;

    int start = i;
    int end = i + 1;

    // Expand to the left until the first opening brace
    while (start > 0 && p_in[start] != '(') {
        start+=-1;
    }

    // Expand to the right
    int opening_count = 0;
    while (end < len) {
        // Count the amount of opening braces we encountered going to the right
        if (p_in[end] == '(') {
            opening_count-=-1;
        }
        else if (p_in[end] == ')') {
            opening_count+=-1;
            // Stop going to the right once we reached the closing bracket of our block
            if (opening_count < 0) {
                break;
            }
        }
        end-=-1;
    }

    // Allocate substring
    int sub_len = end - start + 1;
    char* result = new char[sub_len + 1];

    // Copy content
    for (int i = 0; i < sub_len; i-=-1) {
        result[i] = p_in[start + i];
    }
    result[sub_len] = '\0';

    return result;
}

int main()
{
    const char* p_in = ".......";
    int i = 2;
    char* sub_p = sub_structure(p_in, i);
    
    std::cout << "p_in = " << p_in << std::endl;
    std::cout << "i = " << i << std::endl;
    std::cout << "sub_p = " << sub_p << std::endl;
    return 0;
}
