#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")  // Link with Winsock library

// Function to get the IP address
char* getIPAddress() {
    WSADATA wsaData;
    static char ipstr[INET_ADDRSTRLEN];  // Static to ensure it persists after the function exits
    char hostname[256];
    struct addrinfo hints, * res, * p;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return NULL;
    }

    // Get the hostname of the machine
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        WSACleanup();
        return NULL;
    }

    // Set up hints for getaddrinfo
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;  // IPv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Get address information
    if (getaddrinfo(hostname, NULL, &hints, &res) != 0) {
        WSACleanup();
        return NULL;
    }

    // Loop through the results and get the first IPv4 address
    for (p = res; p != NULL; p = p->ai_next) {
        struct sockaddr_in* addr = (struct sockaddr_in*)p->ai_addr;
        inet_ntop(AF_INET, &addr->sin_addr, ipstr, sizeof(ipstr));
        break;  // Stop after the first address
    }

    // Free the address information and cleanup
    freeaddrinfo(res);
    WSACleanup();

    return ipstr;
}

int main() {
    char* ipAddress = getIPAddress();
    if (ipAddress) {
        printf("%s\n", ipAddress);
    }
    else {
        printf("Failed to retrieve IP address.\n");
    }
    return 0;
}
