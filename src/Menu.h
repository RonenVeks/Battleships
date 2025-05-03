#ifndef MENU_H
#define MENU_H

#include "IO_Assist.h"

// Menu functions codes:
#define JOIN_CODE       101
#define CREATE_CODE     102
#define EXIT_CODE       103

/*
 * The following function lets the user decide on the menu function they will:
 *      Join An existing game
 *      Create a new game
 *      Exit 
 * Input: None
 * Output: The code of the function the user is willing to activate.
 */
int get_menu_function();

#endif // MENU_H
