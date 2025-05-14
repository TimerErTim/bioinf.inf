#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include <vector>
#include <iostream>

class sudoku {
public:
    // Reads Sudoku from input stream (expects order n first)
    bool read(std::istream& in);

    // Prints Sudoku to output stream (prints grid_size first)
    void print(std::ostream& out);

    // Simplifies the Sudoku by reducing the search space
    bool simplify();

    // Solves the Sudoku using exhaustion (backtracking)
    bool solve();

    // Main interface function to read, simplify, solve, and print
    bool check_sudoku(std::istream& in, std::ostream& out);

private:
    int n; // Order of the Sudoku (e.g., 3 for a 9x9 grid), read from file
    int grid_size; // Actual side length of the grid (n*n)
    std::vector<std::vector<int>> grid;

    // finds the next empty cell
    bool find_empty_cell(int& row, int& col);

    // checks if a number is valid in a cell
    bool is_valid(int row, int col, int num);

    // apply constraint propagation
    bool apply_constraints();

    // check if a number is the only possibility in a cell
    bool check_single_possibility(int row, int col, int& num_to_place);

    // check and insert if num is the only possibility in a row/col/block
    bool fill_unique_in_unit(int num);
};

#endif SUDOKU_HPP 
