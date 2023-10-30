#include "main.h"

VOID retrieveOSVersion(OUT PDWORD pMajorVersion, OUT PDWORD pMinorVersion, OUT PDWORD pBuildVersion) {
	PPEB pPeb = getLocalPeb();
#ifdef _WIN64
	*pMajorVersion = *(PDWORD)((PBYTE)pPeb + 0x118);
	*pMinorVersion = *(PDWORD)((PBYTE)pPeb + 0x11c);
	*pBuildVersion = *(PDWORD)((PBYTE)pPeb + 0x120);
#else
	*pMajorVersion = *(PDWORD)((PBYTE)pPeb + 0x0a4);
	*pMinorVersion = *(PDWORD)((PBYTE)pPeb + 0x0a8);
	*pBuildVersion = *(PDWORD)((PBYTE)pPeb + 0x0ac);
#endif
}