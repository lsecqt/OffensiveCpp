#include <tchar.h>
#include <windows.h>

void getCurrentWorkingDirectory() {
    TCHAR cwd[MAX_PATH];
    if (GetCurrentDirectory(MAX_PATH, cwd)) {
        _tprintf(_T("Current Working Directory: %s\n"), cwd);
    }
    else {
        _ftprintf(stderr, _T("Error retrieving current working directory.\n"));
    }
}

int main() {
    getCurrentWorkingDirectory();
    return 0;
}
