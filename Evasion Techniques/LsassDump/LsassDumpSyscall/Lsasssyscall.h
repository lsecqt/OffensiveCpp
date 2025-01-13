#pragma once

#include <Windows.h>
#include <winternl.h>  // For original definitions of structures and types like CLIENT_ID

// Remove any custom definitions of CLIENT_ID or related types

// Prototypes for the functions with the correct signatures, assuming these signatures match those required by your syscalls
EXTERN_C NTSTATUS ZwOpenProcess10(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, CLIENT_ID* ClientId);
EXTERN_C NTSTATUS ZwClose10(HANDLE Handle);
EXTERN_C NTSTATUS ZwWriteVirtualMemory10(HANDLE ProcessHandle, PVOID BaseAddress, LPCVOID Buffer, SIZE_T BufferSize, PSIZE_T NumberOfBytesWritten);
EXTERN_C NTSTATUS ZwProtectVirtualMemory10(HANDLE ProcessHandle, PVOID* BaseAddress, SIZE_T* NumberOfBytesToProtect, ULONG NewAccessProtection, PULONG OldAccessProtection);
EXTERN_C NTSTATUS ZwQuerySystemInformation10(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);
EXTERN_C NTSTATUS NtCreateFile10(PHANDLE FileHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PIO_STATUS_BLOCK IoStatusBlock, PLARGE_INTEGER AllocationSize, ULONG FileAttributes, ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions, PVOID EaBuffer, ULONG EaLength);

// Global function pointers for dynamic resolution, if needed
// If using these, they should be resolved at runtime based on the system version
typedef NTSTATUS(NTAPI* PFN_ZwOpenProcess)(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, CLIENT_ID* ClientId);
typedef NTSTATUS(NTAPI* PFN_ZwClose)(HANDLE Handle);
typedef NTSTATUS(NTAPI* PFN_ZwWriteVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, LPCVOID Buffer, SIZE_T BufferSize, PSIZE_T NumberOfBytesWritten);
typedef NTSTATUS(NTAPI* PFN_ZwProtectVirtualMemory)(HANDLE ProcessHandle, PVOID* BaseAddress, SIZE_T* NumberOfBytesToProtect, ULONG NewAccessProtection, PULONG OldAccessProtection);
typedef NTSTATUS(NTAPI* PFN_ZwQuerySystemInformation)(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);
typedef NTSTATUS(NTAPI* PFN_NtCreateFile)(PHANDLE FileHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PIO_STATUS_BLOCK IoStatusBlock, PLARGE_INTEGER AllocationSize, ULONG FileAttributes, ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions, PVOID EaBuffer, ULONG EaLength);

// Actual function pointer variables
PFN_ZwOpenProcess g_ZwOpenProcess = nullptr;
PFN_ZwClose g_ZwClose = nullptr;
PFN_ZwWriteVirtualMemory g_ZwWriteVirtualMemory = nullptr;
PFN_ZwProtectVirtualMemory g_ZwProtectVirtualMemory = nullptr;
PFN_ZwQuerySystemInformation g_ZwQuerySystemInformation = nullptr;
PFN_NtCreateFile g_NtCreateFile = nullptr;
