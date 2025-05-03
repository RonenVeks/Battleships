#include "Menu.h"   
#include "Communication.h"
#include "Player.h"

/*
 * Compilation line:
gcc -o Battleships src/Battleships.c src/IO_Assist.h src/IO_Assist.c src/Menu.h src/Menu.c src/Communication.h src/Communication.c src/Player.h src/Player.c -lws2_32 -liphlpapi
 */

int
main(void) {
    SOCKET user_socket;
    WSADATA wsa_data;
    //int menu_function = get_menu_function();
    
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data)) {
        PRINT_ERROR("WSAStartup failed");
        return EXIT_FAILURE;
    }

    player_t* p = initialize_player(&user_socket);
    put_ships(p);
    char* to_string = serialize_board(p);
    printf("%s\n", to_string);
    free(to_string);
    free_player(p);

    /*switch (menu_function) {
        case JOIN_CODE:
            connect_to_partner(&user_socket);
            break;
        case CREATE_CODE:
            accept_partner(&user_socket);
            break;
        case EXIT_CODE:
            break;
    }*/
    return EXIT_SUCCESS;
}
