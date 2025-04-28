#include "IO_Assist.h"

int 
_getch() {
    struct termios oldt, newt;
    int ch;

    // Get current terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Disable canonical mode and echo
    newt.c_lflag &= ~(ICANON | ECHO);

    // Apply the new settings
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Read a character
    ch = getchar();

    // Restore old settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}

bool 
check_key_validation(int* keys, int amount, int checked) {
    for (int k = 0; k < amount; k++)
        if (keys[k] == checked) 
            return true;
    return false;
}
