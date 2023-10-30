#include "main.h"

PPEB getLocalPeb() {
#ifdef _WIN64
	return (PPEB)(__readgsqword(0x60));
#else
	return (PPEB)(__readfsdword(0x30));
#endif
}

PPEB getPebWinapi(IN HANDLE hProcess) {
	PROCESS_BASIC_INFORMATION pbi = { 0 };
	PEB peb = { 0 };
    if(hProcess == NULL) hProcess = GetCurrentProcess();
	RtlSecureZeroMemory(&peb, sizeof(PEB));
	RtlSecureZeroMemory(&pbi, sizeof(PROCESS_BASIC_INFORMATION));
	pNtQueryInformationProcess ntQueryInformationProcess = (pNtQueryInformationProcess)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");
	ntQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(PROCESS_BASIC_INFORMATION), NULL);
	ReadProcessMemory(hProcess, pbi.PebBaseAddress, &peb, sizeof(PEB), NULL); 
	return &peb;
}