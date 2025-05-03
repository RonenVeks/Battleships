#include "Menu.h"   

/*
 * Compilation line:
gcc -o Battleships src/Battleships.c src/IO_Assist.h src/IO_Assist.c src/Menu.h src/Menu.c src/Communication.h src/ src/Communication.c
 */

int
main(void) {
    printf("%d\n", get_menu_function());
    return EXIT_SUCCESS;
}
