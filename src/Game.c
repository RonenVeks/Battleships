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
attack_opponent(player_t* p_user, player_t* p_opponent) {
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
        else if (p_opponent->p_marked->hidden) cell_selected = true;
    }

    // Attacking opponent
    buffer[0] = (int)('0') + row;
    buffer[1] = (int)('0') + column;

    if (send(*p_user->p_socket, buffer, BUFFER_SIZE, 0)) {
        PRINT_ERROR("Unable to attack opponent");
        return false;
    }

    // Receiving opponent response
    while (recv(*p_user->p_socket, buffer, BUFFER_SIZE, 0) <= 0) {}

    // Change board according to the opponent's response
    p_opponent->p_marked->value = buffer[0] == HIT_CODE ? HIT : MISS;
    p_opponent->p_marked->marked = false;
    return true;
}
