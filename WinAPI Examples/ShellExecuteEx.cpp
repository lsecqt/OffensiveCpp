#include <windows.h>

int main()
{
	SHELLEXECUTEINFOW info = { sizeof(SHELLEXECUTEINFOW) };
	info.lpFile = L"cmd.exe";
	info.lpParameters = L"/c whoami";
	info.nShow = SW_HIDE;
	info.lpVerb = L"open";
	::ShellExecuteEx(&info);
}
