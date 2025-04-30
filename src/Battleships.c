#include "Menu.h"   

/*
 * Compilation line:
gcc -o Battleships src/Battleships.c src/IO_Assist.h src/IO_Assist.c src/Menu.h src/Menu.c
 */

int
main(void) {
    printf("%d\n", get_menu_function());
    return EXIT_SUCCESS;
}
