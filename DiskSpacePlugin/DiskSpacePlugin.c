
#include <windows.h>


__declspec(dllexport)
void GetAvailableParameters(LPTSTR buf, int bufsize)
{
	GetLogicalDriveStrings(bufsize, buf);
}

__declspec(dllexport)
void GetCurrentValue(LPCTSTR parameter, LPTSTR buf, int bufsize)
{
	DWORD sectorsPerCluster, bytesPerSector, clusters, freeClusters;
	DWORD freeBytes, freeKiloBytes;

	GetDiskFreeSpace(parameter, &sectorsPerCluster, &bytesPerSector, &freeClusters, &clusters);

	freeBytes = freeClusters * sectorsPerCluster * bytesPerSector;
	freeKiloBytes = freeBytes >> 10;

	wsprintf(buf, TEXT("%s %d KB free"), parameter, freeKiloBytes);
}