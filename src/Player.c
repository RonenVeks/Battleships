#include "Player.h"

player_t*
initialize_player(SOCKET* p_socket, bool created) {
    int row, column;
    cell_t* p_cell;
    ship_t* p_ship;
    
    // Allocating the player
    player_t* new_player = (player_t*)malloc(sizeof(player_t));
    if (!new_player) 
        goto allocation_failure;

    // Setting default parameters
    new_player->p_socket = p_socket;
    new_player->turn = created;

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
            p_cell->hidden = false;
        }
    }

    // Setting the top left cell to be the mark
    new_player->p_marked = &(new_player->board[0][0]);

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
    if (cell.hidden)
        cell.marked ? printf("%s%c", RESET, SHIP_ASCII) : printf(" ");
    else switch (cell.value) {
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

        display_single_row(p_player->board[line]);
        printf("\n");
    }

    while (line < BOARD_SIZE) {
        printf("               ");
        display_single_row(p_player->board[line]);
        printf("\n");
        line++;
    }
}

/*
 * The following function ensures that a potential ship is safe to put in 
 * the place it's currently at.
 * Input: A pointer to the player and a pointer to the selected ship.
 * Output: A boolean value that indicates whether or not putting the ship is safe.
 */
static inline bool
safe_to_put(player_t* p_player, ship_t* p_ship) {
    position_t position;

    for (int ship_cell = 0; ship_cell < p_ship->length; ship_cell++) {
        position = p_ship->positions[ship_cell];

        if (p_player->board[position.row][position.column].value != PLAN_DIFFERENCE)
            return false;
    }

    return true;
}

/*
 * The following function allows the user to move the ship they've selected 
 * as they will, move it around and rotate it.
 * Input: A pointer to the player and a pointer to the selected ship.
 * Output: None.
 */
static void
place_specific_ship(player_t* p_player, ship_t* p_ship) {
    bool done = false;
    int root_row = 0, root_column = 0, ship_cell, key;
    position_t position;

    int keys[] = { KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT, KEY_ENTER, (int)('r') };

    while (!done) {
        key = 0;

        // Update the positions of each of the ship's cells
        for (ship_cell = 0; ship_cell < p_ship->length; ship_cell++) {
            if (p_ship->horizontal_orientation) {
                p_ship->positions[ship_cell].row = root_row;
                p_ship->positions[ship_cell].column = root_column + ship_cell;
                p_player->board[root_row][root_column + ship_cell].value += PLAN_DIFFERENCE;
            } else {
                p_ship->positions[ship_cell].row = root_row + ship_cell;
                p_ship->positions[ship_cell].column = root_column;
                p_player->board[root_row + ship_cell][root_column].value += PLAN_DIFFERENCE;
            }
        }

        // Displaying
        display_ships_menu(p_player, 0);
        printf("               %s(R) Rotate ship%s\n", BLUE, RESET);

        // User input
        while (!check_key_validation(keys, 6, key)) key = _getch();

        // Ship position checking
        if (key == KEY_ENTER && safe_to_put(p_player, p_ship)) {
            done = true;
            continue;
        }
        
        // Ship movement
        else if (key == KEY_UP && root_row > 0) root_row--;
        else if (key == KEY_DOWN) {
            // Horizontal
            if ((p_ship->horizontal_orientation && root_row < BOARD_SIZE - 1) ||
                    // Vertical
                    (!p_ship->horizontal_orientation && root_row + p_ship->length < BOARD_SIZE))
                root_row++;
        } else if (key == KEY_RIGHT) {
            // Horizontal
            if ((p_ship->horizontal_orientation && root_column + p_ship->length < BOARD_SIZE) ||
                    // Vertical
                    (!p_ship->horizontal_orientation && root_column < BOARD_SIZE - 1))
                root_column++;
        } else if (key == KEY_LEFT && root_column > 0)
            root_column--;

        // Ship Rotation
        else if (key == (int)('r')) {
            if ((p_ship->horizontal_orientation && root_row + p_ship->length <= BOARD_SIZE) ||
                    (!p_ship->horizontal_orientation && root_column + p_ship->length <= BOARD_SIZE))
                p_ship->horizontal_orientation = !p_ship->horizontal_orientation;
        }

        // Removing the planning marks
        for (ship_cell = 0; ship_cell < p_ship->length; ship_cell++) {
            position = p_ship->positions[ship_cell];
            p_player->board[position.row][position.column].value -= PLAN_DIFFERENCE;
        }
    }

    // Actually putting the ship on the board
    for (ship_cell = 0; ship_cell < p_ship->length; ship_cell++) {
        position = p_ship->positions[ship_cell];
        p_player->board[position.row][position.column].value = p_ship->serial_number;
    }
    p_ship->put = true;
}

void 
put_ships(player_t* p_player) {
    int option = 1, key, ships_put = 0;
    bool ship_placement = false;

    int keys[] = { KEY_UP, KEY_DOWN, KEY_ENTER, (int)('q') };

    while (ships_put < SHIPS_AMOUNT) {
        key = 0;

        display_ships_menu(p_player, option);

        while (!check_key_validation(keys, 4, key)) key = _getch();

        if (!ship_placement) { // Ship selection mode
            if (key == KEY_UP && option > 1) option--;
            else if (key == KEY_DOWN && option < 4) option++;
            else if (key == KEY_ENTER) {
                if (option == 2 && !p_player->ships[option - 1].put &&
                        !p_player->ships[SHIPS_AMOUNT - 1].put) {
                    place_specific_ship(p_player, &p_player->ships[SHIPS_AMOUNT - 1]);
                    ships_put++;
                }
                else if (!p_player->ships[option - 1].put) {
                    place_specific_ship(p_player, &p_player->ships[option - 1]);
                    ships_put++;
                }
            } else if (key == (int)('q')) ships_put = SHIPS_AMOUNT;
        }
    }
}

char* 
serialize_board(player_t* p_player) {
    int column;

    // Allocate Memory
    char* to_string = (char*)calloc(SERIALIZED_LENGTH, sizeof(char));
    if (!to_string) {
        PRINT_ERROR("Memory allocation failed");
        return NULL;
    }

    // Pushing every cell into the string
    for (int row = 0; row < BOARD_SIZE; row++)
        for (column = 0; column < BOARD_SIZE; column++)
            to_string[(row * BOARD_SIZE) + column] = (char)((int)('0') + p_player->board[row][column].value);

    return to_string;
}

/*
 * The following function iterates through a player's array of ships and 
 * find the ship with the specified serial number.
 * Input: A pointer to the player and the serial number.
 * Output: A pointer to the ship with the specified serial number.
 */
static ship_t*
find_ship_by_number(player_t* p_player, int number) {
    for (int ship = 0; ship < SHIPS_AMOUNT; ship++)
        if (p_player->ships[ship].serial_number == number)
            return &(p_player->ships[ship]);
    return NULL;
}

player_t* 
deserialize_board(player_t* p_player, char* to_string) {
    int column, cell;
    ship_t* p_ship;

    // Initializing the opponent
    player_t* opponent = initialize_player(p_player->p_socket, !p_player->turn);
    if (!opponent)
        return NULL;

    // An array to keep track of the indexes of every ship
    int ships_indexes[SHIPS_AMOUNT] = { 0 };

    // Deserializing board string
    for (int row = 0; row < BOARD_SIZE; row++)
        for (column = 0; column < BOARD_SIZE; column++) {
            cell = (int)(to_string[(row * BOARD_SIZE) + column]) - (int)('0'); 

            opponent->board[row][column].value = cell;
            opponent->board[row][column].hidden = true;

            // If ship detected
            if (cell > 0) {
                p_ship = find_ship_by_number(opponent, cell);
                p_ship->positions[ships_indexes[cell - 1]].row = row;
                p_ship->positions[ships_indexes[cell - 1]].column = column;
                p_ship->length = cell == 5 ? 3 : cell + 1;
                ships_indexes[cell - 1]++;
            }
        }

    return opponent;
}
