#include "main.h"
VOID spoofPeb() {
	PPEB pPeb = getLocalPeb();
	const wchar_t spoof[] = L"C:\\Windows\\System32\\notepad.exe";
	const wchar_t spoofDir[] = L"C:\\Windows\\System32";
	USHORT spoodSize = wcslen(spoof) * sizeof(wchar_t);
	USHORT spoodDirSize = wcslen(spoofDir) * sizeof(wchar_t);
	pPeb->ProcessParameters->CommandLine.Buffer = (PWSTR)spoof;
	pPeb->ProcessParameters->CommandLine.Length = spoodSize;
	pPeb->ProcessParameters->ImagePathName.Buffer = (PWSTR)spoof;
	pPeb->ProcessParameters->ImagePathName.Length = spoodSize;
	PCURDIR pCurDir = (PCURDIR)((PBYTE)pPeb->ProcessParameters + 0x038);
	pCurDir->DosPath.Buffer = (PWSTR)spoofDir;
	pCurDir->DosPath.Length = spoodDirSize;
}