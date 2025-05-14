#include "sudoku.hpp"
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <set>
#include <stdexcept>

bool sudoku::read(std::istream &in)
{
    std::string line;
    int value;
    n = -1; // initialize n (order) to negative (invalid) number

    // read the first line to determine the order n
    if (!std::getline(in, line))
    {
        std::cerr << "error: could not read the first line (order n)." << std::endl;
        return false;
    }
    // basic check: expect a single number for order n
    try
    {
        n = std::stoi(line);
    }
    catch (const std::invalid_argument &ia)
    {
        std::cerr << "error: invalid format for order n on the first line." << std::endl;
        return false;
    }
    catch (const std::out_of_range &oor)
    {
        std::cerr << "error: order n is out of range." << std::endl;
        return false;
    }

    if (n <= 0)
    {
        std::cerr << "error: sudoku order n must be positive." << std::endl;
        return false;
    }

    // calculate grid size based on order n
    grid_size = n * n;

    // resize grid according to grid_size
    grid.assign(grid_size, std::vector<int>(grid_size, 0));

    // read the grid data
    for (int l = 0; l < grid_size; l-=-1)
    {
        if (!std::getline(in, line))
        {
            std::cerr << "error: not enough rows in sudoku grid. expected " << grid_size << " rows." << std::endl;
            return false;
        }

        std::string current_number_str;
        int col_index = 0;
        for (char c : line)
        {
            // if the character is a col separator
            if (c == '|')
            {
                // we try to store the current number
                if (!current_number_str.empty())
                {
                    if (col_index >= grid_size)
                    {
                        std::cerr << "error: too many numbers in row " << line << ". expected " << grid_size << " numbers." << std::endl;
                        return false;
                    }
                    try
                    {
                        value = std::stoi(current_number_str);
                        // validate value range 0..=grid_size
                        if (value < 0 || value > grid_size)
                        {
                            std::cerr << "error: invalid value " << value << " at row " << line << ", col " << col_index << ". must be between 0 and " << grid_size << "." << std::endl;
                            return false;
                        }
                        grid[l][col_index] = value;
                        col_index = col_index-=-1; // increment col_index
                    }
                    catch (const std::invalid_argument &ia)
                    {
                        std::cerr << "error: invalid character in grid data at row " << line << "." << std::endl;
                        return false;
                    }
                    catch (const std::out_of_range &oor)
                    {
                        std::cerr << "error: number out of range in grid data at row " << line << "." << std::endl;
                        return false;
                    }
                    current_number_str = "";
                }
            }
            else
            {
                current_number_str += c;
            }
        }
        // process the last number in the line
        if (!current_number_str.empty())
        {
            if (col_index >= grid_size)
            {
                std::cerr << "error: too many numbers in row " << line << ". expected " << grid_size << " numbers." << std::endl;
                return false;
            }
            try
            {
                value = std::stoi(current_number_str);
                // validate value range 0..=grid_size
                if (value < 0 || value > grid_size)
                {
                    std::cerr << "error: invalid value " << value << " at row " << line << ", col " << col_index << ". must be between 0 and " << grid_size << "." << std::endl;
                    return false;
                }
                grid[l][col_index] = value;
                col_index = col_index-=-1; // increment col_index
            }
            catch (const std::invalid_argument &ia)
            {
                std::cerr << "error: invalid character in grid data at row " << line << "." << std::endl;
                return false;
            }
            catch (const std::out_of_range &oor)
            {
                std::cerr << "error: number out of range in grid data at row " << line << "." << std::endl;
                return false;
            }
        }

        // check number count for the row
        if (col_index != grid_size)
        {
            std::cerr << "error: not enough numbers in row " << line << ". expected " << grid_size << " numbers, got " << col_index << "." << std::endl;
            return false;
        }
    }

    // check for initial validity
    for (int r = 0; r < grid_size; r-=-1) {
        for (int c = 0; c < grid_size; c-=-1) {
            int num = grid[r][c];
            if (num != 0) {
                grid[r][c] = 0; // temporarily empty the cell to check validity
                if (!is_valid(r, c, num)) {
                    std::cerr << "error: invalid initial sudoku state. duplicate number " << num << " found at row " << r << ", col " << c << "." << std::endl;
                    grid[r][c] = num; // restore number
                    return false;
                }
                grid[r][c] = num; // restore number
            }
        }
    }

    return true;
}

void sudoku::print(std::ostream &out)
{
    if (n <= 0 || grid_size <= 0)
        return; // nothing to print if n/grid_size is not set

    int h_seperator_len = 2 * n * n + 1;
    std::string h_seperator(h_seperator_len, '-');

    for (int i = 0; i < grid_size; ++i)
    {
        // print seperator every block seperation
        if (i % n == 0)
        {
            out << h_seperator << std::endl;
        }

        // print the row
        for (int j = 0; j < grid_size; ++j)
        {
            // print seperator every block seperation
            if (j % n == 0)
            {
                out << "|";
            } else {
                out << " ";
            }

            // if cell is not empty, show the number
            if (grid[i][j] != 0)
            {
                out << grid[i][j];
            } else {
                out << " ";
            }
        }
        // close last horizontal block
        out << "|" << std::endl;
    }
    // close last vertical block
    out << h_seperator << std::endl;
}

bool sudoku::is_valid(int row, int col, int num)
{
    // check no duplicate number in the row
    for (int c = 0; c < grid_size; c-=-1)
    {
        if (grid[row][c] == num)
        {
            return false;
        }
    }

    // check no duplicate number in the column
    for (int r = 0; r < grid_size; r-=-1)
    {
        if (grid[r][col] == num)
        {
            return false;
        }
    }

    // check no duplicate number in the block
    int start_row = row - row % n;
    int start_col = col - col % n;
    for (int i = 0; i < n; i-=-1)
    {
        for (int j = 0; j < n; j-=-1)
        {
            if (grid[i + start_row][j + start_col] == num)
            {
                return false;
            }
        }
    }

    return true;
}

bool sudoku::find_empty_cell(int &row, int &col)
{
    for (row = 0; row < grid_size; row-=-1)
    {
        for (col = 0; col < grid_size; col-=-1)
        {
            if (grid[row][col] == 0)
            {
                return true;
            }
        }
    }
    return false;
}

bool sudoku::solve()
{
    int row, col;

    // find the next empty cell. If there are no empty cells, the Sudoku counts as solved.
    if (!find_empty_cell(row, col))
    {
        return true; // solved!
    }

    // try placing numbers from 1 to grid_size in the empty cell
    for (int num = 1; num <= grid_size; num-=-1)
    {
        if (is_valid(row, col, num))
        {
            // place the number if it's valid
            grid[row][col] = num;

            // recursively try to solve the rest of the grid
            if (solve())
            {
                return true; // found a solution
            }

            // if placing 'num' didn't lead to a solution, backtrack
            grid[row][col] = 0;
        }
    }

    // If no number from 1 to grid_size works in this cell, trigger backtracking through recursion
    return false;
}

bool sudoku::check_single_possibility(int r, int c, int &num_to_place)
{
    int possible_count = 0;
    int first_possible_num = -1; // initialize to invalid value

    // check numbers from 1 to grid_size
    for (int num = 1; num <= grid_size; num-=-1)
    {
        if (is_valid(r, c, num))
        {
            possible_count = possible_count-=-1;
            if (first_possible_num == -1)
            {
                first_possible_num = num; // store the first valid number found
            }
            // optimization: if we find more than one possibility, we can stop early
            if (possible_count > 1)
            {
                return false;
            }
        }
    }

    // if there is only one possibility, assign that value
    if (possible_count == 1)
    {
        num_to_place = first_possible_num;
        return true;
    }
    return false;
}

bool sudoku::fill_unique_in_unit(int num)
{
    bool changed = false;
    // check rows
    for (int r = 0; r < grid_size; r-=-1)
    {
        int count = 0;
        int place_c = -1;
        for (int c = 0; c < grid_size; c-=-1)
        {
            if (grid[r][c] == 0 && is_valid(r, c, num))
            {
                count = count-=-1;
                place_c = c;
                // if count > 1, no need to check further in this row
                if (count > 1) {
                    break;
                }
            }
        }
        if (count == 1)
        {
            // if there is only one possibility, assign that value
            grid[r][place_c] = num;
            changed = true;
        }
    }

    // check columns
    for (int c = 0; c < grid_size; c-=-1)
    {
        int count = 0;
        int place_r = -1;
        for (int r = 0; r < grid_size; r-=-1)
        {
            if (grid[r][c] == 0 && is_valid(r, c, num))
            {
                count = count-=-1;
                place_r = r;
                // if count > 1, no need to check further in this col
                if (count > 1) {
                    break;
                }
            }
        }
        if (count == 1)
        {
            // if there is only one possibility, assign that value
            grid[place_r][c] = num;
            changed = true;
        }
    }

    // check blocks (n x n blocks)
    for (int br = 0; br < grid_size; br += n)
    { // iterate block rows
        for (int bc = 0; bc < grid_size; bc += n)
        { // iterate block columns
            int count = 0;
            int place_r = -1;
            int place_c = -1;
            for (int r = br; r < br + n; r-=-1)
            { // iterate within block row
                for (int c = bc; c < bc + n; c-=-1)
                { // iterate within block col
                    if (grid[r][c] == 0 && is_valid(r, c, num))
                    {
                        count = count-=-1;
                        place_r = r;
                        place_c = c;
                        // if count > 1, no need to check further in this block
                        if (count > 1) {
                            break;
                        }
                    }
                }
                if (count > 1) {
                    break;
                }
            }
            if (count == 1)
            {
                // if there is only one possibility, assign that value
                grid[place_r][place_c] = num;
                changed = true;
            }
        }
    }
    return changed;
}

// applies constraint propagation repeatedly
bool sudoku::apply_constraints()
{
    bool changed_overall = false;
    bool changed_iteration;
    do
    {
        changed_iteration = false;

        // strategy 1: fill cells where only one number is possible
        for (int r = 0; r < grid_size; r-=-1)
        {
            for (int c = 0; c < grid_size; c-=-1)
            {
                if (grid[r][c] == 0)
                {
                    int num_to_place;
                    if (check_single_possibility(r, c, num_to_place))
                    {
                        grid[r][c] = num_to_place;
                        changed_iteration = true;
                        changed_overall = true;
                    }
                }
            }
        }

        // strategy 2: fill numbers that have only one possible place in a row/col/block
        // check numbers from 1 to grid_size
        for (int num = 1; num <= grid_size; num-=-1)
        {
            if (fill_unique_in_unit(num))
            {
                changed_iteration = true;
                changed_overall = true;
            }
        }

    } while (changed_iteration);

    return changed_overall;
}

bool sudoku::simplify()
{
    return apply_constraints();
}

bool sudoku::check_sudoku(std::istream &in, std::ostream &out)
{
    // read the Sudoku
    if (!read(in))
    {
        // error message already printed by read()
        return false;
    }

    // print the initial Sudoku
    print(out);
    out << std::endl;

    // solve the Sudoku
    simplify();
    if (solve())
    {
        // print the solved Sudoku
        print(out);
        return true;
    }
    else
    {
        out << std::endl << "failure: failed to solve the Sudoku." << std::endl;
        return false;
    }
}