#include "main.h"
BOOL isNtGlobalFlagSet() {
	#define FLG_HEAP_ENABLE_TAIL_CHECK 0x10
	#define FLG_HEAP_ENABLE_TREE_CHECK 0x20
	#define FLG_HEAP_VALIDATE_PARAMETERS 0x40
	PPEB pPeb = getLocalPeb();
#ifdef _WIN64
	DWORD ntGlobalFlag = *(PDWORD)((PBYTE)pPeb + 0xBC);
#else 
	DWORD ntGlobalFlag = *(PDWORD)((PBYTE)pPeb + 0x68);
#endif
	if (ntGlobalFlag == FLG_HEAP_ENABLE_TAIL_CHECK | FLG_HEAP_ENABLE_TREE_CHECK | FLG_HEAP_VALIDATE_PARAMETERS) return true;
}