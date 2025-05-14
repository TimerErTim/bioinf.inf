// example_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "sudoku.hpp"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    sudoku s;
    // default filename
    std::string filename = "../../assets/sudoku-I-3.txt";

    if (argc > 1) {
        filename = argv[1]; // use filename from argument if provided
    }

    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "error: could not open file " << filename << std::endl;
        return 1;
    }

    bool solved = s.check_sudoku(infile, std::cout);

    if (solved) {
        std::cout << std::endl << "Sudoku solved successfully!" << std::endl;
    } else {
        std::cout << std::endl << "Could not solve the Sudoku." << std::endl;
    }

    infile.close();
    return 0;
}
