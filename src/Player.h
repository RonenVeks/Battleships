#ifndef PLAYER_H
#define PLAYER_H

#include <WinSock2.h>
#include "IO_Assist.h"

#define BOARD_SIZE 10
#define SHIPS_AMOUNT 5

// Cell values (1-5 aare ship serial numbers)
#define WATER 0
#define HIT -1
#define MISS -2

// Plan ships will be increased by this number for easier displaying
#define PLAN_DIFFERENCE 6

// Ship ASCII displaying value
#define SHIP_ASCII (char)(178)

// The length of a serialized board
#define SERIALIZED_LENGTH 100

typedef struct {
    int row, column;
} position_t;

typedef struct {
    int serial_number, length;
    position_t* positions;
    bool horizontal_orientation, put, moved;
} ship_t;

typedef struct {
    int value;
    position_t position;
    bool marked, hidden;
} cell_t;

typedef struct {
    cell_t** board, *p_marked;
    ship_t* ships;
    SOCKET* p_socket;
    bool turn;
} player_t;

/*
 * The following function will initialize a new player.
 * Input: A pointer to the socket of the user.
 * Output: A pointer to the newely allocated player.
 */
player_t* initialize_player(SOCKET* p_socket, bool created);

/*
 * The following function will free the allocation of a given player.
 * Input: A pointer to the player.
 * Output: None.
 */
void free_player(player_t* p_player);

/*
 * The following function will print out a specific row out of the player's board.
 * Input: The row that will be printed.
 * Output: None.
 */
void display_single_row(cell_t* row);

/*
 * The following function will allow the player to put ships on the board.
 * Input: A pionter to the player.
 * Output: None.
 */
void put_ships(player_t* p_player);

/*
 * The following function seralizes the player's board into a string.
 * Input: A pointer to the player;
 * Output: A serialized string representing the player's board.
 */
char* serialize_board(player_t* p_player);

/*
 * The following function deserializes a given board string.
 * Input: A pointer to the already existing player and the
 * serialized board string.
 * Output: A pointer to a newely allocated player.
 */
player_t* deserialize_board(player_t* p_player, char* to_string);

// Temporary function to display the board
//void display_board(player_t* p_player);

#endif // PLAYER_H
