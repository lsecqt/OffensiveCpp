#include <windows.h>
#include <stdio.h>
#include <tchar.h>

void PrintPrivileges(HANDLE hToken) {
    DWORD dwSize = 0;
    // First call to GetTokenInformation to get the required buffer size
    if (!GetTokenInformation(hToken, TokenPrivileges, NULL, 0, &dwSize) && GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
        _tprintf(_T("Failed to get token information. Error: %lu\n"), GetLastError());
        return;
    }

    PTOKEN_PRIVILEGES pPrivileges = (PTOKEN_PRIVILEGES)malloc(dwSize);
    if (!pPrivileges) {
        _tprintf(_T("Memory allocation failed.\n"));
        return;
    }

    // Second call to actually get the token information
    if (GetTokenInformation(hToken, TokenPrivileges, pPrivileges, dwSize, &dwSize)) {
        _tprintf(_T("Privileges:\n"));
        for (DWORD i = 0; i < pPrivileges->PrivilegeCount; i++) {
            LUID_AND_ATTRIBUTES la = pPrivileges->Privileges[i];
            TCHAR name[256];  // Buffer to store the privilege name
            DWORD nameSize = sizeof(name) / sizeof(TCHAR);  // Initial buffer size

            // We need to check if the buffer is large enough
            if (!LookupPrivilegeName(NULL, &la.Luid, name, &nameSize)) {
                // If the buffer is too small, reallocate and try again
                if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
                    nameSize = nameSize * 2;  // Double the buffer size
                    name[0] = '\0';  // Clear buffer
                    if (!LookupPrivilegeName(NULL, &la.Luid, name, &nameSize)) {
                        _tprintf(_T(" - Failed to retrieve full privilege name. Error: %lu\n"), GetLastError());
                        continue;
                    }
                }
                else {
                    _tprintf(_T(" - Unknown Privilege (Error %lu)\n"), GetLastError());
                    continue;
                }
            }

            // Print the privilege name and its status
            _tprintf(_T(" - %-35s : %s\n"), name, (la.Attributes & SE_PRIVILEGE_ENABLED) ? _T("Enabled") : _T("Disabled"));
        }
    }
    else {
        _tprintf(_T("Failed to retrieve token privileges. Error: %lu\n"), GetLastError());
    }
    free(pPrivileges);
}

int main() {
    HANDLE hToken;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        _tprintf(_T("Failed to open process token.\n"));
        return 1;
    }

    // Get Username and Domain
    DWORD size = 0;
    GetTokenInformation(hToken, TokenUser, NULL, 0, &size);

    PTOKEN_USER pTokenUser = (PTOKEN_USER)malloc(size);
    if (GetTokenInformation(hToken, TokenUser, pTokenUser, size, &size)) {
        SID_NAME_USE sidType;
        TCHAR username[256];
        TCHAR domain[256];
        DWORD usernameLen = sizeof(username) / sizeof(TCHAR);
        DWORD domainLen = sizeof(domain) / sizeof(TCHAR);

        if (LookupAccountSid(NULL, pTokenUser->User.Sid, username, &usernameLen, domain, &domainLen, &sidType)) {
            _tprintf(_T("Username: %-20s\n"), username);
            _tprintf(_T("Domain:   %-20s\n"), domain);
        }
        else {
            _tprintf(_T("Failed to retrieve account name. Error: %lu\n"), GetLastError());
        }
    }
    else {
        _tprintf(_T("Failed to retrieve token information. Error: %lu\n"), GetLastError());
    }
    free(pTokenUser);

    // Get Token Privileges
    PrintPrivileges(hToken);

    CloseHandle(hToken);
    return 0;
}
