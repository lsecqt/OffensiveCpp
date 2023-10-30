#include "main.h"
HMODULE customGetModuleHandle(const wchar_t* szDllName) {
	PPEB pPeb = getLocalPeb();
	PEB_LDR_DATA* pLdr = (PEB_LDR_DATA*)pPeb->Ldr;
	LIST_ENTRY* pModuleList = (LIST_ENTRY*)&pLdr->InMemoryOrderModuleList;
	LIST_ENTRY* pFirstEntry = pModuleList->Flink;
	for (LIST_ENTRY* pListEntry = pFirstEntry; pListEntry != pModuleList; pListEntry = pListEntry->Flink) {
		LDR_DATA_TABLE_ENTRY* pEntry = (LDR_DATA_TABLE_ENTRY*)pListEntry;
		if (lstrcmpW(szDllName, pEntry->FullDllName.Buffer) == 0) {
			LIST_ENTRY* pInInitializationOrderLinks = (LIST_ENTRY*)((PBYTE)pEntry + 0x020); //0x020 is offset of InInitializationOrderLinks
			return (HMODULE)pInInitializationOrderLinks->Flink;
			//return (HMODULE)pEntry->Reserved2[0]
		}
	}
}