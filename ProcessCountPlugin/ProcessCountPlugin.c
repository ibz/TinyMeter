
#include <windows.h>
#include <tlhelp32.h>

__declspec(dllexport)
void GetAvailableParameters(LPTSTR buf, int bufsize)
{
	TCHAR param[] = TEXT("COUNT\0\0");
	memcpy(buf, param, sizeof(param));
}

__declspec(dllexport)
void GetCurrentValue(LPCTSTR parameter, LPTSTR buf, int bufsize)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 entry;
	ULONG count = 0;

	entry.dwSize = sizeof(PROCESSENTRY32);
	if(Process32First(hSnapshot, &entry))
	{
		count++;

		while(Process32Next(hSnapshot, &entry))
		{
			count++;
		}
	}

	CloseHandle(hSnapshot);

	wsprintf(buf, TEXT("%d processes"), count);
}