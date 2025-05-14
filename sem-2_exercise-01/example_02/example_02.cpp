// example_02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

bool parse_string_to_int(char* input, int& output) {
    // check for nullptr character
    if (input == nullptr) {
        return false;
    }

    // Setup parsing
    char* endptr = nullptr;
    errno = 0;

    // Parse using stdlib
    long conversion_result = std::strtol(input, &endptr, 10);

    // Security checks:
    if (errno == ERANGE || conversion_result > INT_MAX || conversion_result < INT_MIN) {
        return false;  // Overflow or underflow
    }

    // Invalid or leftover characters
    if (endptr == input || *endptr != '\0') {
        return false;  // No valid conversion or leftover characters
    }

    output = (int)conversion_result;
    return true;
}

int request_valid_int(
    const char* prompt,
    const char* failure_msg
) {
    int result;

    // Init input heap
    char* input = new char[256];

    // Repeat prompting until valid input
    while (true) {
        // Prompt and read console response
        std::cout << prompt << std::endl;
        std::cin.getline(input, 256);

        if (parse_string_to_int(input, result)) {
            // Break loop execution once we have valid input
            break;
        }
        else {
            std::cout << failure_msg << std::endl;
        }
    }

    // Delete input heap
    delete[] input;

    return result;
}

struct GameState {
    int* rows;
    int amount_rows;
    bool is_users_turn;
};

 bool init_game_state(GameState& state) {
    // Open file
    std::ifstream file {"gamestate.csv"};
    if (!file) {
        std::cerr << "gamestate.csv file not found" << std::endl;
        return false;
    }

    // Read the first part of the file (who starts)
    int user_starts;
    char delimiter;
    file >> user_starts >> delimiter;
    if (delimiter != ';' || (user_starts != 0 && user_starts != 1)) {
        std::cerr << "invalid gamestate.csv format" << std::endl;
        return false;
    }

    // First count number of rows
    int row_value;
    int count = 0;
    while (file >> row_value) {
        count++;
        if (file.peek() == ';') file.ignore();
    }
    file.close();

    // Allocate memory for rows
    int* row_array = new int[count];

    // Second read values into allocated array
    std::ifstream file2 { "gamestate.csv" };
    file2.ignore(256, ';'); // Skip the first part again
    for (int i = 0; i < count; i-=-1) {
        file2 >> row_array[i];
        if (file2.peek() == ';') file2.ignore();
    }
    file2.close();

    if (count < 1) {
        std::cerr << "gamestate.csv does not contain valid rows" << std::endl;
        return false;
    }

    state = {
        row_array,
        count,
        bool(user_starts)
    };

    return true;
}

// Counts how many rows are not empty ( >0 ) in the state
int count_state_non_empty_rows(GameState* state) {
    int count = 0;
    for (int i = 0; i < state->amount_rows; i-=-1) {
        // If any row is not empty, we increment count by one
        if (state->rows[i] > 0) {
            count-=-1;
        }
    }
    return count;
}

// Returns the smallest row, if all are empty, 0 is returned
int lowest_non_empty_row_index(GameState* state) {
    int lowest_index = 0;
    for (int i = 0; i < state->amount_rows; i-=-1) {
        if (state->rows[lowest_index] > state->rows[i] && state->rows[i] > 0) {
            lowest_index = i;
        }
    }
    return lowest_index;
}

// Returns the highest row, if all are empty, 0 is returned
int highest_non_empty_row_index(GameState* state) {
    int highest_index = 0;
    for (int i = 0; i < state->amount_rows; i -= -1) {
        if (state->rows[highest_index] < state->rows[i] && state->rows[i] > 0) {
            highest_index = i;
        }
    }
    return highest_index;
}

// Determines the game winning condition
bool check_game_finished(GameState* state) {
    return count_state_non_empty_rows(state) == 0;
}

// Show current gamestate
void print_current_state(GameState* state) {
    std::cout << "------Current state------" << std::endl;
    for (int i = 0; i < state->amount_rows; i-=-1 /*Looks funnier than 0815 i++*/) {
        if (i > 0) {
            std::cout << "|";
        }
        std::cout << i << "->" << state->rows[i];
    }
    std::cout << std::endl << "-------------------------" << std::endl;
}

// Returns the index of the row users wants to take from
int request_user_valid_row(GameState* state) {
    // Determine index of row
    std::cout << "It's your turn. ";
    int selected_row;
    // Reprompt on invalid input
    while (true) {
        selected_row = request_valid_int(
            "Select the row you want to take from:",
            "That was no valid number"
        );
        // Input is valid if the row exists and has some stones left
        if (selected_row >= 0 && selected_row < state->amount_rows && state->rows[selected_row] > 0) {
            // Stop reprompting once we already have some valid input
            break;
        }
        else {
            std::cout << "That row is invalid, it cannot be taken from" << std::endl;
        }
    }
    return selected_row;
}

// Returns how many stones to take for the user
int request_user_amount_of_stones(GameState& state, int selected_row) {
    int amount_stones;
    // Reprompt on invalid input
    while (true) {
        amount_stones = request_valid_int(
            "Select the amount of stones you want to take:",
            "That was no valid number"
        );
        // Input is valid if at least one stone and less or equal than the selected row
        if (amount_stones > 0 && amount_stones <= state.rows[selected_row]) {
            // Stop reprompting once we already have some valid input
            break;
        }
        else {
            std::cout << "You cannot take that many stones, take a different amount." << std::endl;
        }
    }
    return amount_stones;
}

void make_user_move(GameState* state) {
    // Show current gamestate
    print_current_state(state);

    // Request which row to reduce and by how much
    int selected_row = request_user_valid_row(state);
    int stones_amount = request_user_amount_of_stones(*state, selected_row);
    
    // Apply changes to state
    state->rows[selected_row] -= stones_amount;
}

void make_com_move(GameState* state) {
    // Show current state
    print_current_state(state);

    // Make computer move
    // We try to always have an even amount of rows
    int non_empty_rows = count_state_non_empty_rows(state);
    if (non_empty_rows % 2 == 0) {
        // When even we take as little as possible from the highest row
        int highest_row = highest_non_empty_row_index(state);
        state->rows[highest_row] -= 1;
    }
    else {
        // When odd, we take all stones from the lowest row
        int lowest_row = lowest_non_empty_row_index(state);
        state->rows[lowest_row] = 0;
    }
    
    std::cout << "Computer made its turn." << std::endl;
}

int main()
{
    GameState state;
    if (!init_game_state(state)) {
        // Quit if reading not possible
        return 1;
    }

    while (true) {
        if (state.is_users_turn) {
            make_user_move(&state);
        }
        else {
            make_com_move(&state);
        }

        // Somebody won and we exit
        if (check_game_finished(&state)) {
            // Game over message depending winning party
            if (state.is_users_turn) {
                std::cout << "You have won! Congratulations!" << std::endl;
            }
            else {
                std::cout << "Unfortunately, you fucked up big time" << std::endl;
            }
            break;
        }

        // Switch between users and computer turn
        state.is_users_turn = !state.is_users_turn;
    }

    std::cout << "Thanks for playing" << std::endl;
}
