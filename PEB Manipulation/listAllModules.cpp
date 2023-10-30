#include "main.h"
VOID listAllModules() {
	PPEB pPeb = getLocalPeb();
	PEB_LDR_DATA* pLdr = (PEB_LDR_DATA*)pPeb->Ldr;
	LIST_ENTRY* pModuleList = (LIST_ENTRY*)&pLdr->InMemoryOrderModuleList;
	LIST_ENTRY* pFirstEntry = pModuleList->Flink;
	for (LIST_ENTRY* pListEntry = pFirstEntry; pListEntry != pModuleList; pListEntry = pListEntry->Flink) {
		LDR_DATA_TABLE_ENTRY* pEntry = (LDR_DATA_TABLE_ENTRY*)pListEntry;
		printf("%S\n", pEntry->FullDllName.Buffer);
	}
}