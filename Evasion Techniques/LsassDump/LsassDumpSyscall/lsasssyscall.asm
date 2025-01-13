.CODE
ZwOpenProcess10 proc
		mov r10, rcx
		mov eax, 26h
		syscall
		ret
ZwOpenProcess10 endp

ZwClose10 proc
		mov r10, rcx
		mov eax, 0Fh
		syscall
		ret
ZwClose10 endp

ZwWriteVirtualMemory10 proc
		mov r10, rcx
		mov eax, 3Ah
		syscall
		ret
ZwWriteVirtualMemory10 endp

ZwProtectVirtualMemory10 proc
		mov r10, rcx
		mov eax, 50h
		syscall
		ret
ZwProtectVirtualMemory10 endp

ZwQuerySystemInformation10 proc
		mov r10, rcx
		mov eax, 36h
		syscall
		ret
ZwQuerySystemInformation10 endp

NtAllocateVirtualMemory10 proc
		mov r10, rcx
		mov eax, 18h
		syscall
		ret
NtAllocateVirtualMemory10 endp

NtFreeVirtualMemory10 proc
		mov r10, rcx
		mov eax, 1Eh
		syscall
		ret
NtFreeVirtualMemory10 endp

NtCreateFile10 proc
		mov r10, rcx
		mov eax, 55h
		syscall
		ret
NtCreateFile10 endp

end
