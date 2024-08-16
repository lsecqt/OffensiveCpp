#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <tlhelp32.h>

DWORD GetProcessIdByName(IN const char* name, OUT HANDLE& hProcess)
{
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    char buf[MAX_PATH] = { 0 };
    size_t charsConverted = 0;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (snapshot == INVALID_HANDLE_VALUE)
    {
        printf("[-] Could not create snapshot. Error: %lu\n", GetLastError());
        return NULL;
    }

    if (Process32First(snapshot, &entry) == TRUE)
    {
        do
        {
            wcstombs_s(&charsConverted, buf, MAX_PATH, entry.szExeFile, MAX_PATH - 1);
            if (_stricmp(buf, name) == 0)
            {
                hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
                if (hProcess != NULL)
                {
                    printf("[+] Got handle on %d\n", entry.th32ProcessID);
                    CloseHandle(snapshot); // Always close handles you no longer need
                    return entry.th32ProcessID;
                }
                printf("[-] Could not obtain handle on %d. Continuing!\n", entry.th32ProcessID);
            }
        } while (Process32Next(snapshot, &entry) == TRUE);
    }

    printf("[-] Process not found.\n");
    CloseHandle(snapshot);
    return NULL;
}

int main(int argc, char* argv[])
{
	int pid = GetProcessIdByName(argv[1]);
	printf("%d\n", pid);
}
