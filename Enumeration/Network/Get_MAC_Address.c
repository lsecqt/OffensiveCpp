#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib, "ws2_32.lib")     // Link with Winsock library
#pragma comment(lib, "iphlpapi.lib")  // Link with IP Helper API library


// Function to get the MAC address
char* getMACAddress() {
    static char macstr[18];  // Static to ensure it persists after the function exits
    PIP_ADAPTER_INFO adapterInfo;
    PIP_ADAPTER_INFO adapter;
    DWORD bufferSize = sizeof(IP_ADAPTER_INFO);
    adapterInfo = (IP_ADAPTER_INFO*)malloc(bufferSize);

    // Get the adapter info
    if (GetAdaptersInfo(adapterInfo, &bufferSize) == ERROR_BUFFER_OVERFLOW) {
        free(adapterInfo);
        adapterInfo = (IP_ADAPTER_INFO*)malloc(bufferSize);
    }

    if (GetAdaptersInfo(adapterInfo, &bufferSize) == NO_ERROR) {
        // Loop through adapters and get the first MAC address
        for (adapter = adapterInfo; adapter != NULL; adapter = adapter->Next) {
            snprintf(macstr, sizeof(macstr), "%02X:%02X:%02X:%02X:%02X:%02X",
                adapter->Address[0], adapter->Address[1], adapter->Address[2],
                adapter->Address[3], adapter->Address[4], adapter->Address[5]);
            break;  // Stop after the first adapter
        }
    }
    else {
        free(adapterInfo);
        return NULL;
    }

    free(adapterInfo);
    return macstr;
}

int main() {
    
    char* macAddress = getMACAddress();

    if (macAddress) {
        printf("MAC Address: %s\n", macAddress);
    }
    else {
        printf("Failed to retrieve MAC address.\n");
    }

    return 0;
}
