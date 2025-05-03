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

/*
 * The following function will print out a specific cell according to it's value.
 * Input: The cell that should be displayed.
 * Output: None.
 */
static void
display_cell(cell_t cell) {
    switch (cell.value) {
        case WATER:
            cell.marked ? printf("%s%s~%s", BG_BLUE, BLACK, RESET) : printf("%s~", BLUE);
            break;
        case HIT:
            cell.marked ? printf("%s%sX%s", BG_RED, BLACK, RESET) : printf("%s%sX%s", BG_WHITE, RED, RESET);
            break;
        case MISS:
            cell.marked ? printf("%s%s#%s", BG_GREEN, BLACK, RESET) : printf("%s#", GREEN);
            break;
        case PLAN_DIFFERENCE:
            printf("%s%c", GREEN, SHIP_ASCII);
            break;
        default:
            printf("%s%c", cell.value > PLAN_DIFFERENCE ? RED : RESET, SHIP_ASCII);
            break;
    }
    printf("%s|", YELLOW);
}

/*
 * The following function will print out a specific row out of the player's board.
 * Input: The row that will be printed.
 * Output: None.
 */
void
display_single_row(cell_t* row) {
    printf("%s|", YELLOW);

    for (int column = 0; column < BOARD_SIZE; column++)
        display_cell(row[column]);

    printf("%s", RESET); // Reseting the colors
}

/*
 * The following function prints out the ship placement menu in a nice and comfortable way.
 * Input: A pointer to the player and the ship option they're currently on.
 * Output: None.
 */
static void
display_ships_menu(player_t* p_player, int option) {
    int line;

    CLEAR_TERMINAL;

    printf("%sShips Left:     %s___________________\n", MAGENTA, YELLOW);

    // Print out ships sizes
    for (line = 0; line < SHIPS_AMOUNT - 1; line++) {
        option == line + 1 ? printf("%s -> ", RESET) : printf("    ");

        if (line == 1) { // There are 2 ships at the size of 3 cells
            if ((!p_player->ships[1].put) && (!p_player->ships[SHIPS_AMOUNT - 1].put))// If the user didn't put none of the 3 cell-long ships
                printf("%s3 Cells x2 ", GREEN);
            else (p_player->ships[1].put && p_player->ships[SHIPS_AMOUNT - 1].put) ? 
            printf("%s3 Cells    ", RED) : printf("%s3 Cells    ", GREEN);
        } else printf("%s%d Cells    ", p_player->ships[line].put ? RED : GREEN,line + 2);
    }

}

void 
put_ships(player_t* p_player) {
}
