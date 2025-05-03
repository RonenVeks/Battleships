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
#define PLAN_DIFFERENCE

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
    bool marked;
} cell_t;

typedef struct {
    cell_t** board, *p_marked;
    ship_t* ships;
    SOCKET* p_socket;
} player_t;

/*
 * The following function will initialize a new player.
 * Input: A pointer to the socket of the user.
 * Output: A pointer to the newely allocated player.
 */
player_t* initialize_player(SOCKET* p_socket);

#endif // PLAYER_H
