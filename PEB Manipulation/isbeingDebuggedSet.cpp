#include "main.h"

BOOL isBeingDebuggedSet() {
	PPEB pPeb = getLocalPeb();
	if (pPeb->BeingDebugged == 1) return true;
}