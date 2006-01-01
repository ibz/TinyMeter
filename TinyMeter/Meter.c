#include <windows.h>
#include "Meter.h"

Meter* MeterCreate(LPCTSTR pluginName, LPCTSTR parameter)
{
	Meter* meter = (Meter*)malloc(sizeof(Meter));
	int size = (lstrlen(pluginName) + 1) * sizeof(TCHAR);
	meter->PluginName = (LPTSTR)malloc(size);
	lstrcpy(meter->PluginName, pluginName);
	if(parameter)
	{
		size = (lstrlen(parameter) + 1) * sizeof(TCHAR);
		meter->Parameter = (LPTSTR)malloc(size);
		lstrcpy(meter->Parameter, parameter);
	}
	else
	{
		meter->Parameter = 0;
	}
	meter->Position = -1;
	meter->Visible = TRUE;

	return meter;
}