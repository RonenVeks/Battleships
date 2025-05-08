#include "Game.h"

void 
display_both_boards(player_t* p_user, player_t* p_opponent) {
    CLEAR_TERMINAL;

    printf("%sYou:\t\t\t\tOpponent:\n", MAGENTA);
    for (int row = 0; row < BOARD_SIZE; row++) {
        display_single_row(p_user->board[row]);
        printf("\t\t");
        display_single_row(p_opponent->board[row]);
        printf("\n");
    }
}

/*
 * The following function moves the mark of the board of a given player.
 * Input: A pointer to the player and the numbers for the row and the column.
 * Output: None.
 */
static void
change_mark(player_t* p_player, int row, int column) {
    p_player->p_marked->marked = false;
    p_player->p_marked = &(p_player->board[row][column]);
    p_player->p_marked->marked = true;
}

bool
attack_opponent(player_t* p_user, player_t* p_opponent, char* end_code) {
    bool cell_selected = false;
    int key, row, column;
    char buffer[BUFFER_SIZE];

    // Valid keys
    int keys[] = { KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT, KEY_ENTER };

    while (!cell_selected) {
        row = p_opponent->p_marked->position.row;
        column = p_opponent->p_marked->position.column;
        key = 0;

        display_both_boards(p_user, p_opponent);

        while (!check_key_validation(keys, 5, key)) key = _getch();

        // Mark movement
        if (key == KEY_UP && row > 0) change_mark(p_opponent, row - 1, column);
        else if (key == KEY_DOWN && row < BOARD_SIZE - 1) change_mark(p_opponent, row + 1, column);
        else if (key == KEY_RIGHT && column < BOARD_SIZE - 1) change_mark(p_opponent, row, column + 1);
        else if (key == KEY_LEFT && column > 0) change_mark(p_opponent, row, column - 1);
        else if (key == KEY_ENTER && p_opponent->p_marked->hidden) cell_selected = true;
    }

    // Attacking opponent
    buffer[0] = (int)('0') + row;
    buffer[1] = (int)('0') + column;

    if (send(*p_user->p_socket, buffer, BUFFER_SIZE, 0) == SOCKET_ERROR) {
        PRINT_ERROR("Unable to attack opponent");
        return false;
    }

    // Receiving opponent response
    while (recv(*p_user->p_socket, buffer, BUFFER_SIZE, 0) <= 0) {}

    // Change board according to the opponent's response
    if (buffer[0] == DEFEAT_CODE)
        *end_code = WIN_CODE;
    else {
        p_opponent->p_marked->value = buffer[0] == HIT_CODE ? HIT : MISS;
        p_opponent->p_marked->hidden = false;
        p_opponent->p_marked->marked = false;
    }
    return true;
}

/*
 * The following function iterates through the cells of every ship and checks if they 
 * are all hit - meaning defeat.
 * Input: A pointer to the player.
 * Output: A boolean value that indicates whether or not was the player defeated.
 */
static bool
check_defeat(player_t* p_player) {
    int pos;
    ship_t* p_ship;

    for (int ship_index = 0; ship_index < SHIPS_AMOUNT; ship_index++) {
            p_ship = &(p_player->ships[ship_index]);
            /*if (p_player->board[p_player->ships[pos].positions[pos].row]
                    [p_player->ships[pos].positions[pos].column].value != HIT)
                return false;*/
            if (p_player->board[p_ship->positions[pos].row][p_ship->positions[pos].column].value != HIT)
                return false;
    }
    return true;
}

bool
get_attacked(player_t* p_user, char* end_code) {
    int row, column;
    char buffer[BUFFER_SIZE];

    // Receiving attack
    while (recv(*p_user->p_socket, buffer, BUFFER_SIZE, 0) <= 0) {}

    row = buffer[0] - (int)('0');
    column = buffer[1] - (int)('0');

    // Sending a response
    if (p_user->board[row][column].value > 0) {
        p_user->board[row][column].value = HIT;
        if (check_defeat(p_user)) {
            buffer[0] = DEFEAT_CODE;
            *end_code = DEFEAT_CODE;
        } else buffer[0] = HIT_CODE;
    } else {
        buffer[0] = MISS_CODE;
        p_user->board[row][column].value = MISS;
    }
    buffer[1] = '0';
    if (send(*p_user->p_socket, buffer, BUFFER_SIZE, 0) == SOCKET_ERROR) {
        PRINT_ERROR("Unable to send response");
        return false;
    }

    return true;
}

void
expose_both_boards(player_t* p_user, player_t* p_opponent) {
    int column; 

    // Revealing each of the cells
    for (int row = 0; row < BOARD_SIZE; row++)
        for (column = 0; column < BOARD_SIZE; column++)
            p_opponent->board[row][column].hidden = false;

    display_both_boards(p_user, p_opponent);
}
