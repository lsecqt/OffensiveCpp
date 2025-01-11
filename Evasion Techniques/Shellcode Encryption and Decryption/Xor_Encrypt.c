#include <windows.h>
#include <stdio.h>

unsigned char buf[] = "non encrypted payload";

void encrypt(char *payload, int payload_length, char *key, int length) {
    int j = 0;

    for (int i = 0; i < payload_length; i++) {
        if (j == length - 1) j = 0;
        payload[i] ^= key[j];
        j++;
    }
}

int main()
{
	char key[] = "test";
	encrypt(buf, sizeof(buf), key, sizeof(key));
}