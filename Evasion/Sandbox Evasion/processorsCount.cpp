#include <Windows.h>

int main()
{
	SYSTEM_INFO info;
	::GetNativeSystemInfo(&info);
	if (info.dwNumberOfProcessors < 2)
	{
		return -99;
	}
}
