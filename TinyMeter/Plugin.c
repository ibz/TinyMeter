#include <windows.h>
#include "Plugin.h"

Plugin* PluginCreate(LPCTSTR pluginName, HMODULE handle, LPFNGETPARAMS getAvailableParameters, LPFNGETVALUE getCurrentValue)
{
	Plugin* plugin = (Plugin*)malloc(sizeof(Plugin));
	int size = (lstrlen(pluginName) + 1) * sizeof(TCHAR);
	plugin->PluginName = (LPTSTR)malloc(size);
	lstrcpy(plugin->PluginName, pluginName);
	plugin->Handle = handle;
	plugin->GetAvailableParameters = getAvailableParameters;
	plugin->GetCurrentValue = getCurrentValue;

	return plugin;
}