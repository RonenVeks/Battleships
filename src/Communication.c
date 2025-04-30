#include "Communication.h"

#define IP_LENGTH 16

/*
 * -- Copied from ChatGPT --
 * The following function prints out the IP address of the user, regardless 
 * of whether the connection is wired or wireless.
 * Input: None.
 * Output: A boolean value indicating whether or not the printing was successfull.
 */
static bool 
print_ip() {
    PIP_ADAPTER_INFO adapter_info, adapter = NULL;
    DWORD buffer_length = sizeof(IP_ADAPTER_INFO);
    bool is_wireless = false;
    char ip_address[IP_LENGTH] = "";

    adapter_info = (IP_ADAPTER_INFO*)malloc(buffer_length);
    if (!adapter_info) {
        PRINT_ERROR("Memory allocation failed\n");
        return false;
    }

    if (GetAdaptersInfo(adapter_info, &buffer_length) == R_ERROR_BUFFER_OVERFLOW) {
        free(adapter_info);
        adapter_info = (IP_ADAPTER_INFO*)malloc(buffer_length);
        if (!adapter_info) {
            PRINT_ERROR("Memory allocation failed\n");
            return false;
        }
    }

    if (GetAdaptersInfo(adapter_info, &buffer_length) == R_NO_ERROR) {
        adapter = adapter_info;
        while (adapter) {
            if (adapter->IpAddressList.IpAddress.String[0] != '\0') { // Ensure IP exists
                if (strstr(adapter->Description, "Wireless") != NULL) {
                    is_wireless = true;
                    strncpy(ip_address, adapter->IpAddressList.IpAddress.String, sizeof(ip_address));
                    break; 
                } else if (!is_wireless && ip_address[0] == '\0')
                    strncpy(ip_address, adapter->IpAddressList.IpAddress.String, sizeof(ip_address));
            }
            adapter = adapter->Next;
        }
    } else {
        PRINT_ERROR("GetAdaptersInfo failed\n");
        free(adapter_info);
        return false;
    }

    if (ip_address[0] != '\0')
        printf("%s\n", ip_address);
    else {
        PRINT_ERROR("No active network adapter found\n");
        free(adapter_info);
        return false;
    }

    free(adapter_info);
    return true;
}
