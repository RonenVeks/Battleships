#ifndef IO_ASSIST
#define IO_ASSIST

#include <stdio.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>

// Colors:

// Reset
#define RESET       "\033[0m"

// Regular Colors
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

// Background Colors
#define BG_BLACK    "\033[40m"
#define BG_RED      "\033[41m"
#define BG_GREEN    "\033[42m"
#define BG_YELLOW   "\033[43m"
#define BG_BLUE     "\033[44m"
#define BG_MAGENTA  "\033[45m"
#define BG_CYAN     "\033[46m"
#define BG_WHITE    "\033[47m"

/*
 * (Copied from ChatGPT)
 * The following function accepts the keyboard input of the user without displaying the pressed key.
 * Input: None.
 * Output: The integer value of the pressed key (ASCII).
 */
int _getch();

/*
 * The following function checkes whether or not a pressed key is in the array of relevant keys.
 * Input: The array of the relevant keys, the amount of relevant keys and the checked key.
 * Output: A boolean value which indicates whether or not the checked key is relevant.
 */
bool check_key_validation(int* keys, int amount, int checked);

#endif // IO_ASSIST
