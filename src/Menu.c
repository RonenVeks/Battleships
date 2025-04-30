#include "Menu.h"

void
print_logo() {
    printf("                                              _     \n");
    printf("                                      /    _,'|     \n");
    printf("                                     /_ _,'   |     \n");
    printf("                                    %s$$%s;'     _;     \n", RED, RESET);
    printf("                    ,-'-._    ,-'. ,-'    _.'\n");
    printf("                    \\     `-,'  ,-'    _,'\n");
    printf("%s   _od8PP8booo%s   ,....       ;,'    ,,'  %s_____\n", BLUE, RESET, BLUE);
    printf("%s d8P''%s         ,'     \\   _,'    ,-' %s,oo8P\"\"\"Y8.\n", BLUE, RESET, BLUE);
    printf("%s `'%s             `-.    i,'    ,-'%s,odPP''      8b\n", BLUE, RESET, BLUE);
    printf("                   %s`-,'    ,-'%s,o8P'          ,8P\n", RESET, BLUE);
    printf("                %s.',-'   ,-'%s,o8P'             d8\n", RESET, BLUE);
    printf("             %s.'.-'  _,-'%s,o8P'               ,8P\n", RESET, BLUE);
    printf("           %s',-'  _,' %s_o8P'                  dP\n", RESET, BLUE);
    printf("8bo,     %s_,'  _,'%s  ,dP'                    d8\n", RESET, BLUE);
    printf("  Y8'    %s\\ ,,'%s  ,o8P'                    _op'                     _,d8P\n", RESET, BLUE);
    printf("  dP      %s'%s  ,o8P'                     ,o8'                   ,oo8P\"'\n", RESET, BLUE);
    printf("  Yb     _oo8P'                      ,dP'                 ,odPP''\n");
    printf("   Ybooo8P''                       ,dP'              _,odPP'\n");
    printf("     ''                           o8'            _oo8P\"'\n");
    printf("                                ,8P         _,odPP''\n");
    printf("                               d8'    __,odPP'\n");
    printf("                               Y8oooo8PP'\n");
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
        else chose = true;
    }

    switch (option) {
        case 1: return JOIN_CODE;
        case 2: return CREATE_CODE;
        case 3: return EXIT_CODE;
        default: return 1;
    }
}
