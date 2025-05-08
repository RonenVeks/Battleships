#ifndef GAME_H
#define GAME_H

#include "Player.h"

#define BUFFER_SIZE 2
#define HIT_CODE '1'
#define MISS_CODE '2'
#define DEFEAT_CODE '3'
#define WIN_CODE '4'


/*
 * The following function prints out both of the board next to each other.
 * Input: A pointer to the user's player and a pointer to the opponent's player.
 * Output: None.
 */
void display_both_boards(player_t* p_user, player_t* p_opponent);

/*
 * The following function allows the user to attack their opponent wherever they'd like.
 * Input: A pointer to the user's player and a pointer to the opponent's player.
 * Output: A boolean value that indicates whether or not everything was successfull.
 */
bool attack_opponent(player_t* p_user, player_t* p_opponent, char* end_code);

/*
 * The following function lets the user to be attacked, processes the attack and respondes accordingly.
 * Input: A pointer to the user's player.
 * Output: A boolean value that indicates whether or not everything was successfull.
 */
bool get_attacked(player_t* p_user, char* end_code);

/*
 * The following function exposes both of the boards next to each other.
 * Input: A pointer to the user's player and a pointer to the opponent's player.
 * Output: None.
 */
void expose_both_boards(player_t* p_user, player_t* p_opponent);


#endif // GAME_H
