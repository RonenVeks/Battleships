#include "Game.h"

void 
display_both_board(player_t* p_user, player_t* p_opponent) {
    CLEAR_TERMINAL;

    printf("%sYou:\t\t\t\tOpponent:\n", MAGENTA);
    for (int row = 0; row < BOARD_SIZE; row++) {
        display_single_row(p_user->board[row]);
        printf("\t\t");
        display_single_row(p_opponent->board[row]);
        printf("\n");
    }
}
