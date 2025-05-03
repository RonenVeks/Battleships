#include "Player.h"

player_t*
initialize_player(SOCKET* p_socket) {
    int row, column;
    cell_t* p_cell;
    ship_t* p_ship;
    
    // Allocating the player
    player_t* new_player = (player_t*)malloc(sizeof(player_t));
    if (!new_player) 
        goto allocation_failure;

    new_player->p_socket = p_socket;

    // Allocating the player's board
    new_player->board = (cell_t**)calloc(BOARD_SIZE, sizeof(cell_t*));
    if (!new_player->board) {
        free(new_player);
        goto allocation_failure;
    }

    // Initializing the cells of the board
    for (row = 0; row < BOARD_SIZE; row++) {
        new_player->board[row] = (cell_t*)calloc(BOARD_SIZE, sizeof(cell_t));
        if (!new_player->board[row]) {
            for (int temp = 0; temp < row; temp++)
                free(new_player->board[temp]);
            free(new_player->board);
            free(new_player);
            goto allocation_failure;
        }

        for (column = 0; column < BOARD_SIZE; column++) {
            p_cell = &new_player->board[row][column];

            p_cell->position = (position_t){ .row = row, .column = column };
            p_cell->value = WATER;
            p_cell->marked = false;
        }
    }

    // Allocating the array of ships
    new_player->ships = (ship_t*)calloc(SHIPS_AMOUNT, sizeof(ship_t));
    if (!new_player->ships)
        goto free_matrix;

    for (int ship = 0; ship < SHIPS_AMOUNT; ship++) {
        p_ship = &new_player->ships[ship];

        p_ship->serial_number = ship + 1;
        p_ship->length = (ship == SHIPS_AMOUNT - 1) ? 3 : (ship + 2);
        p_ship->horizontal_orientation = true;
        p_ship->put = false;
        p_ship->moved = false;
        p_ship->positions = (position_t*)calloc(p_ship->length, sizeof(position_t));
        if (!p_ship->positions) {
            for (int temp = 0; temp < ship; temp++)
                free(new_player->ships[temp].positions);
            free(new_player->ships);
            goto free_matrix;
        }
    }

    return new_player;

free_matrix:
    for (row = 0; row < BOARD_SIZE; row++)
        free(new_player->board[row]);
    free(new_player->board);
    free(new_player);
    goto allocation_failure;

allocation_failure:
    PRINT_ERROR("Memory allocation failed");
    return NULL;
}

void 
free_player(player_t* p_player) {
    // Freeing the allocation of the ships
    for (int ship = 0; ship < SHIPS_AMOUNT; ship++)
        free(p_player->ships[ship].positions);
    free(p_player->ships);

    // Freeing the allocation of the board's matrix
    for (int row = 0; row < BOARD_SIZE; row++)
        free(p_player->board[row]);
    free(p_player->board);
    free(p_player);
}

void 
put_ships(player_t* p_player) {
    int option = 1, key, ships_put = 0;

}
