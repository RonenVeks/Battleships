#include "Menu.h"   
#include "Communication.h"
#include "Game.h"
#include <windows.h>

/*
 * Compilation line:
gcc -o Battleships src/Battleships.c src/IO_Assist.h src/IO_Assist.c src/Menu.h src/Menu.c src/Communication.h src/Communication.c src/Player.h src/Player.c src/Game.h src/Game.c -lws2_32 -liphlpapi
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
    player_t* p_player = (player_t*)lp_player;
    put_ships(p_player);

    int bytes_sent = send(*p_player->p_socket, serialize_board(p_player), SERIALIZED_LENGTH, 0);
    if (bytes_sent == SOCKET_ERROR) {
        PRINT_ERROR("Board sending failed");
        return 1;
    }
    return 0;
}

// Structure for function parameters
typedef struct {
    player_t* p_user, *p_result;
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

    while (recv(*receiving->p_user->p_socket, buffer, SERIALIZED_LENGTH, 0) <= 0) {}

    receiving->p_result = deserialize_board(receiving->p_user, buffer);
    return 0;
}

int
main(void) {
    SOCKET user_socket;
    WSADATA wsa_data;
    bool (*menu_function)(SOCKET*), created;
    player_t* p_player, *p_opponent;

    // Variables for multithreading
    HANDLE h_ships_movement, h_receive_board;
    DWORD ships_movement_id, receive_board_id;

    // Menu function selection
    int menu_function_code = get_menu_function();
    if (menu_function_code == 1) {
        PRINT_ERROR("False input");
        return EXIT_FAILURE;
    }
    
    // Start WSA
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data)) {
        PRINT_ERROR("WSAStartup failed");
        return EXIT_FAILURE;
    }

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
            WSACleanup();
            return EXIT_SUCCESS;
    }

    // Activating the menu function
    if (!menu_function(&user_socket))
        goto failure;

    p_player = initialize_player(&user_socket, created);
    if (!p_player)
        goto failure;

    // Creating the threads
    h_ships_movement = CreateThread(NULL, 0, move_ships, p_player, 0, NULL);
    receive_result_t* params = (receive_result_t*)malloc(sizeof(receive_result_t));
    if (!params) {
        PRINT_ERROR("Memory allocation failed");
        free_player(p_player);
        goto failure;
    }
    params->p_user = p_player;
    params->p_result = NULL;
    h_receive_board = CreateThread(NULL, 0, receive_board, params, 0, NULL);

    if (!h_ships_movement || !h_receive_board) {
        PRINT_ERROR("Couldn't create threads");
        free(params);
        free_player(p_player);
        goto failure;
    }

    // Activating the threads
    WaitForSingleObject(h_ships_movement, INFINITE);
    WaitForSingleObject(h_receive_board, INFINITE);

    // Closing threads
    CloseHandle(h_ships_movement);
    CloseHandle(h_receive_board);

    p_opponent = params->p_result;
    free(params);

    // display_board(p_opponent); // Might be needed again in the future
    display_both_board(p_player, p_opponent);

    free_player(p_opponent);
    free_player(p_player);

    return EXIT_SUCCESS;

failure:
    WSACleanup();
    return EXIT_FAILURE;
}
