#include <Windows.h>
#include <tlhelp32.h>
#include <stdio.h>

int main()
{
	int pid = 0;
	LPCTSTR  processname = L"svchost.exe";
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);

	if (!Process32First(hSnapshot, &pe))
	{
		-1;
	}

	HANDLE hProcess = NULL;
	do {
		if (0 == _wcsicmp(processname, pe.szExeFile))
		{
			pid = pe.th32ProcessID;
			printf("[!] Trying to open handle on %ls, on pid %d\n", processname, pid);

			hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, false, pid);
			if (hProcess == NULL)
			{
				printf("[X] Could not open handle on %d, continuing\n", pid);
			}
			else
			{
				printf("[+] Successfully got handle on %d\n", pid);
				break;
			}
		}
	} while (Process32Next(hSnapshot, &pe));

	CloseHandle(hSnapshot);
}

