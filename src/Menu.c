#include "Menu.h"

void
print_logo() {
    printf("%s ________  ________  _________  _________  ___       _______   ________  ___  ___  ___  ________  ________      \n", RED);
    printf("|\\   __  \\|\\   __  \\|\\___   ___\\\\___   ___\\\\  \\     |\\  ___ \\ |\\   ____\\|\\  \\|\\  \\|\\  \\|\\   __  \\|\\   ____\\     \n");
    printf("\\ \\  \\|\\ /\\ \\  \\|\\  \\|___ \\  \\_\\|___ \\  \\_\\ \\  \\    \\ \\   __/|\\ \\  \\___|\\ \\  \\\\\\  \\ \\  \\ \\  \\|\\  \\ \\  \\___|_    \n");
    printf(" \\ \\   __  \\ \\   __  \\   \\ \\  \\     \\ \\  \\ \\ \\  \\    \\ \\  \\_|/_\\ \\_____  \\ \\   __  \\ \\  \\ \\   ____\\ \\_____  \\   \n");
    printf("  \\ \\  \\|\\  \\ \\  \\ \\  \\   \\ \\  \\     \\ \\  \\ \\ \\  \\____\\ \\  \\_|\\ \\|____|\\  \\ \\  \\ \\  \\ \\  \\ \\  \\___|\\|____|\\  \\  \n");
    printf("   \\ \\_______\\ \\__\\ \\__\\   \\ \\__\\     \\ \\__\\ \\ \\_______\\ \\_______\\____\\_\\  \\ \\__\\ \\__\\ \\__\\ \\__\\     ____\\_\\  \\ \n");
    printf("    \\|_______|\\|__|\\|__|    \\|__|      \\|__|  \\|_______|\\|_______|\\_________\\|__|\\|__|\\|__|\\|__|    |\\_________\\\n");
    printf("                                                                 \\|_________|                       \\|_________|\n");
    printf("%s                                              _     \n", RESET);
    printf("                                      /    _,'|     \n");
    printf("                                     /_ _,'   |     \n");
    printf("                                    %s$$%s;'     _;    \n", RED, RESET);
    printf("                    ,-'-._    ,-'. ,-'    _.'    \n");
    printf("                    \\     `-,'  ,-'    _,'   \n");
    printf("   %s_od8PP8booo%s   ,....       ;,'    ,,'  %s_____    \n", BLUE, RESET, BLUE);
    printf(" %sd8P''         %s,'     \\   _,'    ,-' %s,oo8P\"""Y8.\n", BLUE, RESET, BLUE);
    printf(" %s`'%s             `-.    i,'    ,-'%s,odPP''      8b \n", BLUE, RESET, BLUE);
    printf("                   %s`-,'    ,-'%s,o8P'          ,8P \n", RESET, BLUE);
    printf("                %s.',-'   ,-'%s,o8P'             d8  \n", RESET, BLUE);
    printf("             %s.'.-'  _,-'%s,o8P'               ,8P  \n", RESET, BLUE);
    printf("           %s',-'  _,' %s_o8P'                  dP  \n", RESET, BLUE);
    printf("%s8bo,     %s_,'  _,'  %s,dP'                    d8  \n", BLUE, RESET, BLUE);
    printf("  %sY8'    %s\\ ,,'  %s,o8P'                    _op'                     _,d8P\n", BLUE, RESET, BLUE);
    printf("  %sdP      %s'  %s,o8P'                     ,o8'                   ,oo8P\"'\n", BLUE, RESET, BLUE);
    printf("  Yb     _oo8P'                      ,dP'                 ,odPP''\n");
    printf("   Ybooo8P''                       ,dP'              _,odPP'\n");
    printf("     ''                           o8'            _oo8P\"'\n");
    printf("                                ,8P         _,odPP''\n");
    printf("                               d8'    __,odPP\"'\n");
    printf("                               Y8oooo8PP\"'\n");
    printf("                                ~~~~~~%s\n", RESET);
}

int
get_menu_function() {
    int keys[] = { KEY_UP, KEY_DOWN, KEY_ENTER }, key, option = 1;
    bool chose = false;

    while (!chose) {
        key = 0;

        CLEAR_TERMINAL;

        print_logo();
        
        printf("%s%sJoin Game%s\n", option == 1 ? " -> " : "    ", GREEN, RESET);
        printf("%s%sCreate Game%s\n", option == 2 ? " -> " : "    ", YELLOW, RESET);
        printf("%s%sExit%s\n", option == 3 ? " -> " : "    ", RED, RESET);

        while (!check_key_validation(keys, 3, key)) key = _getch();
        if (key == KEY_UP && option > 1) option--;
        else if (key == KEY_DOWN && option < 3) option++;
        else if (key == KEY_ENTER) chose = true;
    }

    switch (option) {
        case 1: return JOIN_CODE;
        case 2: return CREATE_CODE;
        case 3: return EXIT_CODE;
        default: return 1;
    }
}
