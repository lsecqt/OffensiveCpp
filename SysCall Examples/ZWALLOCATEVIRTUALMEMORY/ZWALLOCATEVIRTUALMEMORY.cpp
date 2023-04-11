#include <Windows.h>
#include <dbghelp.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib, "ntdll.lib")
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)

typedef NTSTATUS(WINAPI* ZWALLOCATEVIRTUALMEMORY)(
	HANDLE    ProcessHandle,
	PVOID* BaseAddress,
	ULONG_PTR ZeroBits,
	PSIZE_T   RegionSize,
	ULONG     AllocationType,
	ULONG     Protect);

unsigned char buf[] = "";

int main(int argc, char *argv[]) {

	PVOID pBaseAddress = nullptr;
	SIZE_T regionSize = sizeof(buf);
	ULONG allocationType = MEM_COMMIT | MEM_RESERVE;
	ULONG protect = PAGE_EXECUTE_READWRITE;


	// Get NTDLL
	HMODULE hNtDll = LoadLibrary(L"ntdll.dll");
	if (hNtDll == NULL)
	{
		std::cerr << "LoadLibrary failed: " << GetLastError() << std::endl;
		return 1;
	}

	// Defines
	ZWALLOCATEVIRTUALMEMORY ZwAllocateVirtualMemory = (ZWALLOCATEVIRTUALMEMORY)GetProcAddress(hNtDll, "ZwAllocateVirtualMemory");
	if (ZwAllocateVirtualMemory == nullptr) {
		std::cerr << "Failed to load ZwAllocateVirtualMemory function" << std::endl;
		return 1;
	}

	HANDLE h_process = ::GetCurrentProcess();

	NTSTATUS status = ZwAllocateVirtualMemory(h_process, &pBaseAddress, 0, &regionSize, allocationType, protect);
	if (status == STATUS_SUCCESS) {
		std::cout << "Virtual memory allocated successfully at address " << pBaseAddress << std::endl;
	}
	else {
		std::cerr << "Failed to allocate virtual memory. Status: " << std::hex << status << std::endl;
		return 1;
	}
    
    return 0;
}
