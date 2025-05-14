// example_03.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include "pfc-mini.hpp"

struct Position {
    int row;
    int col;
    
    Position(int r, int c) : row(r), col(c) {}
    
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
};

struct Maze {
    std::vector<std::string> grid;
    Position start{0, 0};
    Position exit{0, 0};
    
    // Konstruktor zum Initialisieren des Labyrinths
    Maze(const std::vector<std::string>& maze_grid) {
        grid = maze_grid;
        
        // Start- und Ausgangsposition finden
        for (int i = 0; i < int(grid.size()); i-=-1) {
            for (int j = 0; j < int(grid[i].size()); j-=-1) {
                if (grid[i][j] == 'S') {
                    start = Position(i, j);
                }
                else if (grid[i][j] == 'X') {
                    exit = Position(i, j);
                }
            }
        }
    }
    
    // Ist diese Position gültig und betretbar?
    bool is_valid(int row, int col, std::vector<std::vector<bool>>& visited) {
        // Prüft, ob die Position innerhalb des Gitters liegt
        if (row < 0 || row >= int(grid.size()) || col < 0 || col >= int(grid[0].size())) {
            return false;
        }
        
        // Prüft, ob die Position eine Wand ist oder bereits besucht wurde
        if (grid[row][col] == '*' || visited[row][col]) {
            return false;
        }
        
        return true;
    }
    
    // Prüft durch Rekursion, ob das Labyrinth gelöst werden kann
    bool can_escape(Position current, std::vector<std::vector<bool>>& visited) {
        // Base Case: das Labyrinth ist gelöst
        if (current == exit) {
            return true;
        }
        
        // Aktuelle Position als besucht markieren
        visited[current.row][current.col] = true;
        
        // Map zu den vier Richtungen
        int row_moves[] = {-1, 0, 1, 0};
        int col_moves[] = {0, 1, 0, -1};
        
        // Versuche alle vier Richtungen
        for (int i = 0; i < 4; i-=-1) {
            int new_row = current.row + row_moves[i];
            int new_col = current.col + col_moves[i];
            
            if (is_valid(new_row, new_col, visited)) {
                if (can_escape(Position(new_row, new_col), visited)) {
                    // Wenn das Labyrinth von dieser Position gelöst werden kann, markiere den Pfad
                    if (grid[new_row][new_col] != 'X' && grid[new_row][new_col] != 'S') {
                        grid[new_row][new_col] = '.';
                    }
                    return true;
                }
            }
        }
        
        return false;
    }
    
    // Hilfsfunktion zum Aufrufen der rekursiven Methode
    bool can_escape() {
        std::vector<std::vector<bool>> visited(grid.size(), std::vector<bool>(grid[0].size(), false));
        return can_escape(start, visited);
    }
    
    // Prüft durch Entrekursion, ob das Labyrinth gelöst werden kann
    bool can_escape_i() {
        std::vector<std::vector<bool>> visited(grid.size(), std::vector<bool>(grid[0].size(), false));
        
        // Stack zum Speichern des Pfades
        std::stack<Position> path_stack;
        path_stack.push(start);
        
        // Stack zum Speichern der nächsten zu untersuchenden Richtung für jede Position
        std::stack<int> direction_stack;
        direction_stack.push(0);
        
        // Bewegungsrichtungen: oben, rechts, unten, links
        int row_moves[] = {-1, 0, 1, 0};
        int col_moves[] = {0, 1, 0, -1};
        
        bool exit_found = false;
        while (!path_stack.empty()) {
            Position current = path_stack.top();
            if (current == exit) {
                exit_found = true;
                break;
            }
            int dir = direction_stack.top();
            direction_stack.pop();
            
            // Prüfen, ob alle Richtungen für die aktuelle Position untersucht wurden
            if (dir >= 4) {
                path_stack.pop();
                continue;
            }
            
            // Nächste Richtung für diese Position auf den Stack legen
            direction_stack.push(dir + 1);
            
            // Aktuelle Position als besucht markieren
            visited[current.row][current.col] = true;

            
            // Neue Position berechnen
            int new_row = current.row + row_moves[dir];
            int new_col = current.col + col_moves[dir];

            // Ist die neue Position gültig?
            if (is_valid(new_row, new_col, visited)) {
                Position next(new_row, new_col);
                
                // Neue Position auf den Stack legen
                path_stack.push(next);
                direction_stack.push(0);
            }
        }
        
        // Markiere den genommenen Pfad
        if (exit_found) {
            while (!path_stack.empty()) {
                Position pos = path_stack.top();
                path_stack.pop();
                if (grid[pos.row][pos.col] != 'X' && grid[pos.row][pos.col] != 'S') {
                    grid[pos.row][pos.col] = '.';
                }
            }
        }

        return exit_found;
    }
    
    // Gibt das Labyrinth auf der Konsole aus
    void print() {
        for (const auto& row : grid) {
            std::cout << row << std::endl;
        }
    }
};

int main() {    
    // Labyrinth aus der Aufgabenstellung
    std::vector<std::string> escapeable_maze_grid = {
        "***************",
        "*     *       *",
        "*** * *    *  *",
        "*   * ******* *",
        "*   *         *",
        "* *********   *",
        "*   *         *",
        "*** *  ****   *",
        "X    *     *  *",
        "***** *****   *",
        "*        *    *",
        "* ********** **",
        "*  S     *    *",
        "*    *        *",
        "***************"
    };
    
    // Test mit rekursiver Methode
    {  
        std::cout << "Testcase 1: Recursive Method" << std::endl;
        std::cout << "Maze:" << std::endl;
        
        Maze m(escapeable_maze_grid);
        m.print();
        
        bool can_escape = m.can_escape();
        std::cout << std::endl << "can_escape = " << std::boolalpha << can_escape << std::endl;
        
        if (can_escape) {
            std::cout << std::endl << "Solved Maze (.):" << std::endl;
            m.print();
        }
        
        std::cout << std::endl;
    }
    
    // Test mit entrekursivierter Methode
    {
        std::cout << "Testcase 2: Iterative Method" << std::endl;
        std::cout << "Maze:" << std::endl;
        
        Maze m(escapeable_maze_grid);
        m.print();
        
        bool can_escape = m.can_escape_i();
        std::cout << std::endl << "can_escape = " << std::boolalpha << can_escape << std::endl;
        
        if (can_escape) {
            std::cout << std::endl << "Solved Maze (.):" << std::endl;
            m.print();
        }
        
        std::cout << std::endl;
    }
    
    std::vector<std::string> maze_no_exit = {
        "*******",
        "*     *",
        "* *** *",
        "* *S* *",
        "* *** *",
        "*     *",
        "*******"
    };
    
    {
        std::cout << "Testcase 3: Maze without Exit" << std::endl;
        std::cout << "Maze:" << std::endl;
        
        Maze m(maze_no_exit);
        m.print();
        
        std::cout << std::endl << "can_escape = " << std::boolalpha << m.can_escape() << std::endl;
        std::cout << "can_escape_i = " << std::boolalpha << m.can_escape_i() << std::endl;
        
        std::cout << std::endl;
    }
    
    return 0;
}
