#include "Menu.h"   
#include "Communication.h"
#include "Player.h"
#include <windows.h>

/*
 * Compilation line:
gcc -o Battleships src/Battleships.c src/IO_Assist.h src/IO_Assist.c src/Menu.h src/Menu.c src/Communication.h src/Communication.c src/Player.h src/Player.c -lws2_32 -liphlpapi
 */

/*
 * -- Multithreading --
 * The following function activates a another function that let's the user to arrange
 * the ships on the board as they will.
 * Input: A long void pointer later converted to the pointer to the user's player.
 * Output: A double word that tells Windows the operation was completed successfully.
 */
DWORD WINAPI 
move_ships(LPVOID lp_player) { 
    put_ships((player_t*)lp_player);
    return 0;
}

// Structure for function parameters
typedef struct {
    player_t* p_user, *p_result;
    SOCKET* p_socket;
} receive_result_t;

/*
 * -- Multithreading--
 * The following function receives the opponent's board.
 * Input: A long void pointer later converted into a struct containing 
 * a pointer to the existing user's player, a pointer to the opponent's potential
 * player and the socket.
 * Output: A double word that tells Windows the operation was completed successfully.
 */
DWORD WINAPI
receive_board(LPVOID lp_params) {
    char buffer[SERIALIZED_LENGTH];
    receive_result_t* receiving = (receive_result_t*)lp_params;
    if (recv(*receiving->p_socket, buffer, SERIALIZED_LENGTH, 0) <= 0) {
        PRINT_ERROR("Connection failed");
        return 1;
    }

    deserialize_board(receiving->p_user, buffer);
    return 0;
}

int
main(void) {
    SOCKET user_socket;
    WSADATA wsa_data;
    bool (*menu_function)(SOCKET*), created;
    player_t* p_player;

    // Variables for multithreading
    HANDLE h_ships_movement, h_receive_board;
    DWORD ships_movement_id, receive_board_id;

    // Menu function selection
    int menu_function_code = get_menu_function();
    if (menu_function_code == 1) {
        PRINT_ERROR("False input");
        return EXIT_FAILURE;
    }
    
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data)) {
        PRINT_ERROR("WSAStartup failed");
        return EXIT_FAILURE;
    }

    /*player_t* p = initialize_player(&user_socket, true);
    put_ships(p);
    char* to_string = serialize_board(p);
    printf("%s\n", to_string);
    player_t* o = deserialize_board(p, to_string);
    display_board(o);
    free(to_string);
    free_player(o);
    free_player(p);*/

    // Connecting between the menu selection and the correct function
    switch (menu_function_code) {
        case JOIN_CODE:
            menu_function = connect_to_partner;
            created = false;
            break;
        case CREATE_CODE:
            menu_function = accept_partner;
            created = true;
            break;
        case EXIT_CODE:
            return EXIT_SUCCESS;
    }

    // Activating the menu function
    if (!menu_function(&user_socket))
        return EXIT_FAILURE;

    p_player = initialize_player(&user_socket, created);
    if (!p_player)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
