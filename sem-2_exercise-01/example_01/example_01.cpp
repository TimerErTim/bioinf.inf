// example_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

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
    int m_stapel;
    int n_stapel;
};

int request_m_size() {
    return request_valid_int(
        "Enter the size of m stack",
        "That was no valid number"
    );
}

int request_n_size() {
    return request_valid_int(
        "Enter the size of n stack",
        "That was no valid number"
    );
}

GameState init_game_state() {
    int m_size = request_m_size();
    int n_size = request_n_size();
    GameState state = {
        m_size,
        n_size
    };
    return state;
}


// Determines the game winning condition
bool check_game_finished(GameState* state) {
    return state->m_stapel <= 0 && state->n_stapel <= 0;
}

// Used to display current sizes
void print_current_state(GameState* state) {
    std::cout << "-----Current state-----" << std::endl;
    std::cout << "m -> " << state->m_stapel << "  n -> " << state->n_stapel << std::endl;
    std::cout << "-----------------------" << std::endl;
}


bool is_user_action_valid(GameState* state, int action) {
    if (action > 2 || action < 1) {
        // Invalid option
        return false;
    }
    if ((state->m_stapel == 0 || state->n_stapel == 0) && action == 2) {
        // Invalid when user wants to take both stones but one of them is empty
        return false;
    }
    return true;
}

bool is_user_stack_choise_valid(GameState* state, int stack_index) {
    if (stack_index > 2 || stack_index < 1) {
        // Invalid option
        return false;
    }
    if ((state->m_stapel == 0 && stack_index == 1) || (state->n_stapel == 0 && stack_index == 2)) {
        return false;
    }
    return true;
}

// Returns 1 for single stack, 2 for both stacks
int request_user_valid_action(GameState* state) {
    // Determine type of action
    std::cout << "It's your turn. Select one of the following:" << std::endl;
    int selected_action;
    // Reprompt on invalid input
    while (true) {
        selected_action = request_valid_int(
            "1) Take from one stack\n2) Take from both stacks",
            "That was no valid number"
        );
        if (is_user_action_valid(state, selected_action)) {
            // Stop reprompting once we already have some valid input
            break;
        }
        else {
            std::cout << "That action is not possible" << std::endl;
        }
    }
    return selected_action;
}

//
bool request_user_should_start() {
    // Reprompt on invalid input
    while (true) {
        int selected_action = request_valid_int(
            "Would you like to take the first turn (0/1):",
            "That was no valid number"
        );
        if (selected_action == 0) {
            // The user does not want to start himself
            return false;
        }
        else if (selected_action == 1) {
            // The user wants to start himself
            return true;
        }
        else {
            std::cout << "You have to enter either '0' or '1'" << std::endl;
        }
    }
}

// Returns 1 for single stack, 2 for both stacks
int request_user_valid_stack_choice(GameState* state) {
    // Determine type of action
    std::cout << "Select the stack you want to take from:" << std::endl;
    int selected_stack;
    // Reprompt on invalid input
    while (true) {
        selected_stack = request_valid_int(
            "1) Take from M stack\n2) Take from N stack",
            "That was no valid number"
        );
        if (is_user_stack_choise_valid(state, selected_stack)) {
            // Stop reprompting once we already have some valid input
            break;
        }
        else {
            std::cout << "That stack choice is not valid" << std::endl;
        }
    }
    return selected_stack;
}

// Takes a user game step, return true if user won at that step
bool take_game_step_user(GameState* state) {
    // Show current gamestate
    print_current_state(state);

    int selected_action = request_user_valid_action(state);

    // On 2, we subtract from both stacks
    if (selected_action == 2) {
        state->m_stapel--;
        state->n_stapel--;
    }

    // On 1, we ask which stack to subtract from
    if (selected_action == 1) {
        int selected_stack = request_user_valid_stack_choice(state);
        // Performa stack subtraction accordingly
        if (selected_stack == 1) {
            state->m_stapel--;
        }
        else if (selected_stack == 2) {
            state->n_stapel--;
        }
    }

    print_current_state(state);

    return check_game_finished(state);
}

// Makes the best possible game step on the GameState
void make_move(GameState* state) {
    // Refer to accompanied documentation

    if (state->m_stapel % 2 == 0 && state->n_stapel % 2 == 0) {
        // If both are even, we decrement one that is not zero
        if (state->m_stapel > 0) {
            state->m_stapel--;
        }
        if (state->n_stapel > 0) {
            state->n_stapel--;
        }
        // We return early because we cannot do anything else
        return;
    }

    // Make sure M stack is even
    if (state->m_stapel % 2 != 0 && state->m_stapel > 0) {
        state->m_stapel--;
    }

    // Make sure N stack is even
    if (state->n_stapel % 2 != 0 && state->n_stapel > 0) {
        state->n_stapel--;
    }
}

// Takes a computer game step, returns true if computer won at that step
bool take_game_step_com(GameState* state) {
    std::cout << "The computer is now calculating next step..." << std::endl;

    make_move(state);

    std::cout << "The computer has made its turn" << std::endl;

    return check_game_finished(state);
}

int main()
{
    // We setup the initial game state
    GameState state = init_game_state();
    // Get first party to start
    bool should_user_play = request_user_should_start();

    while (true) {
        // User take his turn
        if (should_user_play && take_game_step_user(&state)) {
            // If won, congratulate and quit
            std::cout << "You have won! Congratulations!" << std::endl;
            break;
        }
        // Computer takes his turn
        if (take_game_step_com(&state)) {
            // If won, congratulate and quit
            std::cout << "The computer has won! Better luck next time!" << std::endl;
            break;
        }
        should_user_play = true;
    }

    std::cout << "Thanks for playing" << std::endl;
}
