#include <windows.h>

#ifndef __METER_H__
#define __METER_H__

typedef struct
{
	LPTSTR PluginName;
	LPTSTR Parameter;
	int Position;
	int Visible;
} Meter;

Meter* MeterCreate(LPCTSTR pluginName, LPCTSTR parameter);

#endif // #ifndef __METER_H__