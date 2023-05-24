#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <tlhelp32.h>

DWORD GetProcessIdByName(const char * name)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	char buf[MAX_PATH] = { 0 };
	size_t charsConverted = 0;

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			wcstombs_s(&charsConverted, buf, entry.szExeFile, MAX_PATH);
			if (_stricmp(buf, name) == 0)
			{
				HANDLE ph = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(entry.th32ProcessID));
				if (ph != NULL)
				{
					printf("[+] Got handle on %d\n", entry.th32ProcessID);
					return entry.th32ProcessID;
				}
				printf("[-] Could not obtain handle on %d. Continuing!\n", entry.th32ProcessID);
			}
		}
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	int pid = GetProcessIdByName(argv[1]);
	printf("%d\n", pid);
}