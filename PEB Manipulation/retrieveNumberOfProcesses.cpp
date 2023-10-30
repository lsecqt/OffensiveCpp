#include "main.h"

DWORD retrieveNumberOfProcesses() {
	PPEB pPeb = getLocalPeb();
	DWORD numberOfProcesses = *(PDWORD)((PBYTE)pPeb + 0x0b8);
	return numberOfProcesses;
}