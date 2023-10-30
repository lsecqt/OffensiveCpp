#pragma once
#include <windows.h>
#include <iostream>
#include <winternl.h>
typedef NTSTATUS(NTAPI* pNtQueryInformationProcess)(
	HANDLE           ProcessHandle,
	PROCESSINFOCLASS ProcessInformationClass,
	PVOID            ProcessInformation,
	ULONG            ProcessInformationLength,
	PULONG           ReturnLength
	);

typedef struct _CURDIR {
	UNICODE_STRING DosPath;
	HANDLE Handle;
} CURDIR, * PCURDIR;

PPEB getLocalPeb();
PPEB getPebWinapi(IN HANDLE hProcess);

