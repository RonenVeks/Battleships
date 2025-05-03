#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "IO_Assist.h"
#include <string.h>
#include <WinSock2.h>
#include <windows.h>
#include <iphlpapi.h>

#pragma comment(lib, "lphlpapi.lib")
#pragma comment(lib, "Ws2_32.lib")

// Taumatawhakatangihangakoauauotamateaturipukakapikimaungahoronukupokaiwhenuakitanatahu
#define PORT 9061

// Re-defining macros to avoid compilation errors
#define R_NO_ERROR 0
#define R_ERROR_BUFFER_OVERFLOW 111

/*
 * The following function waits for the partner to connect.
 * Input: A pointer to the socket on which the connection is established.
 * Output: A boolean value that indicates whether or not was the connection successfull.
 */
bool accept_partner(SOCKET* p_socket);

#endif // COMMUNICATION_H
