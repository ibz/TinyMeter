#include <windows.h>

#ifndef __PLUGIN_H__
#define __PLUGIN_H__

typedef void (* LPFNGETPARAMS)(LPTSTR, int);
typedef void (* LPFNGETVALUE)(LPCTSTR, LPTSTR, int);

typedef struct
{
	LPTSTR PluginName;
	HMODULE Handle;
	LPFNGETPARAMS GetAvailableParameters;
	LPFNGETVALUE GetCurrentValue;
} Plugin;

Plugin* PluginCreate(LPCTSTR pluginName, HMODULE handle, LPFNGETPARAMS getAvailableParameters, LPFNGETVALUE getCurrentValue);

#endif // #ifndef __PLUGIN_H__