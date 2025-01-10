#include <winsock2.h>
#include <windows.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

// Define INET_ADDRSTRLEN if it's not already defined
#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN 22
#endif

void displayNetworkConnections() {
    PMIB_TCPTABLE_OWNER_PID tcpTable = NULL;
    PMIB_UDPTABLE_OWNER_PID udpTable = NULL;
    DWORD tcpTableSize = 0, udpTableSize = 0;
    DWORD result;

    // Retrieve TCP table size
    result = GetExtendedTcpTable(NULL, &tcpTableSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
    tcpTable = (PMIB_TCPTABLE_OWNER_PID)malloc(tcpTableSize);

    if (tcpTable == NULL || GetExtendedTcpTable(tcpTable, &tcpTableSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) != NO_ERROR) {
        printf("Failed to retrieve TCP table.\n");
        free(tcpTable);
        return;
    }

    // Retrieve UDP table size
    result = GetExtendedUdpTable(NULL, &udpTableSize, FALSE, AF_INET, UDP_TABLE_OWNER_PID, 0);
    udpTable = (PMIB_UDPTABLE_OWNER_PID)malloc(udpTableSize);

    if (udpTable == NULL || GetExtendedUdpTable(udpTable, &udpTableSize, FALSE, AF_INET, UDP_TABLE_OWNER_PID, 0) != NO_ERROR) {
        printf("Failed to retrieve UDP table.\n");
        free(tcpTable);
        free(udpTable);
        return;
    }

    printf("Active Network Connections (TCP):\n");
    for (DWORD i = 0; i < tcpTable->dwNumEntries; i++) {
        char localAddr[INET_ADDRSTRLEN], remoteAddr[INET_ADDRSTRLEN];
        struct in_addr local, remote;

        local.S_un.S_addr = tcpTable->table[i].dwLocalAddr;
        remote.S_un.S_addr = tcpTable->table[i].dwRemoteAddr;

        inet_ntop(AF_INET, &local, localAddr, sizeof(localAddr));
        inet_ntop(AF_INET, &remote, remoteAddr, sizeof(remoteAddr));

        printf("TCP: Local: %s:%d -> Remote: %s:%d | State: %ld\n",
            localAddr, ntohs((u_short)tcpTable->table[i].dwLocalPort),
            remoteAddr, ntohs((u_short)tcpTable->table[i].dwRemotePort),
            tcpTable->table[i].dwState);
    }

    printf("\nActive Network Connections (UDP):\n");
    for (DWORD i = 0; i < udpTable->dwNumEntries; i++) {
        char localAddr[INET_ADDRSTRLEN];
        struct in_addr local;

        local.S_un.S_addr = udpTable->table[i].dwLocalAddr;

        inet_ntop(AF_INET, &local, localAddr, sizeof(localAddr));

        printf("UDP: Local: %s:%d\n",
            localAddr, ntohs((u_short)udpTable->table[i].dwLocalPort));
    }

    free(tcpTable);
    free(udpTable);
}

int main() {
    displayNetworkConnections();
    return 0;
}
