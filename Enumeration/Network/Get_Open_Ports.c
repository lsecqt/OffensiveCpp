#include <winsock2.h>  // Include winsock2.h first to avoid conflicts
#include <windows.h>   // Include windows.h after winsock2.h
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")  // Link Winsock library for ntohs

// Function to retrieve all open TCP ports
void getOpenPorts() {
    DWORD bufferSize = 0;
    PMIB_TCPTABLE_OWNER_PID tcpTable = NULL;
    DWORD result;

    // Determine the buffer size required for the TCP table
    result = GetExtendedTcpTable(NULL, &bufferSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
    if (result != ERROR_INSUFFICIENT_BUFFER) {
        printf("Failed to determine buffer size. Error: %lu\n", result);
        return;
    }

    tcpTable = (PMIB_TCPTABLE_OWNER_PID)malloc(bufferSize);
    if (tcpTable == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    // Retrieve the TCP table
    result = GetExtendedTcpTable(tcpTable, &bufferSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
    if (result != NO_ERROR) {
        printf("Failed to retrieve TCP table. Error: %lu\n", result);
        free(tcpTable);
        return;
    }

    // Loop through the table and display open ports
    printf("Open TCP Ports:\n");
    for (DWORD i = 0; i < tcpTable->dwNumEntries; i++) {
        DWORD localPort = ntohs((u_short)tcpTable->table[i].dwLocalPort);
        DWORD state = tcpTable->table[i].dwState;

        // Display ports in LISTENING or ESTABLISHED state
        if (state == MIB_TCP_STATE_LISTEN || state == MIB_TCP_STATE_ESTAB) {
            printf("Port: %lu, State: %s\n",
                localPort,
                (state == MIB_TCP_STATE_LISTEN) ? "LISTENING" : "ESTABLISHED");
        }
    }

    // Free the allocated memory
    free(tcpTable);
}

int main() {
    getOpenPorts();
    return 0;
}
