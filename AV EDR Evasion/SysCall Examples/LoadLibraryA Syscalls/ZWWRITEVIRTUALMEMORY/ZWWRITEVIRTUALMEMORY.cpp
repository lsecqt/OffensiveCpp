#include <Windows.h>
#include <dbghelp.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib, "ntdll.lib")
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)

typedef NTSTATUS(WINAPI* ZWWRITEVIRTUALMEMORY)(
	HANDLE ProcessHandle,
	PVOID BaseAddress,
	PVOID Buffer,
	ULONG BufferSize,
	PULONG NumberOfBytesWritten);

unsigned char buf[] = "";

int main(int argc, char *argv[]) {

	PVOID pBaseAddress = nullptr;
	ULONG bytesWritten = 0;


	// Get NTDLL
	HMODULE hNtDll = LoadLibrary(L"ntdll.dll");
	if (hNtDll == NULL)
	{
		std::cerr << "LoadLibrary failed: " << GetLastError() << std::endl;
		return 1;
	}

	// Defines
	ZWWRITEVIRTUALMEMORY ZwWriteVirtualMemory = (ZWWRITEVIRTUALMEMORY)GetProcAddress(hNtDll, "NtWriteVirtualMemory");
	if (ZwWriteVirtualMemory == nullptr) {
		std::cerr << "Failed to load NtWriteVirtualMemory function" << std::endl;
		return 1;
	}

	HANDLE h_process = ::GetCurrentProcess();
	LPVOID alloc = ::VirtualAllocEx(h_process, NULL, sizeof(buf), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	NTSTATUS status = ZwWriteVirtualMemory(h_process, alloc, buf, sizeof(buf), &bytesWritten);
	if (status == STATUS_SUCCESS) {
		std::cout << "Wrote " << bytesWritten << " bytes to virtual memory at address " << pBaseAddress << std::endl;
	}
	else {
		std::cerr << "Failed to write virtual memory. Status: " << std::hex << status << std::endl;
		return 1;
	}
    
	return 0;
}
