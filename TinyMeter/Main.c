#include <windows.h>
#include "List.h"
#include "MainWindow.h"
#include "Meter.h"
#include "Plugin.h"

#define PARAMS_BUFSIZE_MAX 512

static List* LoadPlugins();
static void UnloadPlugins(List* plugins);
static List* GetMeters(List* plugins);
static void DestroyMeters(List* meters);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	List* plugins = LoadPlugins();
	List* meters = GetMeters(plugins);

	hInstance; hPrevInstance; lpCmdLine; nCmdShow; // get rid of warnings

	MainWindowShow(plugins, meters);

	DestroyMeters(meters);
	ListDestroy(&meters);
	UnloadPlugins(plugins);
	ListDestroy(&plugins);
}

static Plugin* LoadPlugin(LPCTSTR fileName)
{
	HMODULE hLib = LoadLibrary(fileName);

	return PluginCreate(fileName, hLib, (LPFNGETPARAMS)GetProcAddress(hLib, "GetAvailableParameters"), (LPFNGETVALUE)GetProcAddress(hLib, "GetCurrentValue"));
}

static List* LoadPlugins()
{
	List* plugins;
	WIN32_FIND_DATA findFileData;
	HANDLE hFind;

	hFind = FindFirstFile(TEXT("*.dll"), &findFileData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	plugins = ListCreate();
	plugins->Car = LoadPlugin(findFileData.cFileName);
	while(FindNextFile(hFind, &findFileData))
	{
		List* node = ListCreate();
		node->Car = LoadPlugin(findFileData.cFileName);
		ListAppend(&plugins, node);
	}

    FindClose(hFind);

	return plugins;
}

void UnloadPlugins(List* plugins)
{
	List* p = plugins;
	while(p)
	{
		if(p->Car)
		{
			Plugin* plugin = (Plugin*)p->Car;
			if(plugin->PluginName)
			{
				free(plugin->PluginName);
				plugin->PluginName = 0;
			}
			FreeLibrary(plugin->Handle);
			free(plugin);
			p->Car = 0;
		}
		p = p->Cdr;
	}
}

static List* GetMeters(List* plugins)
{
	List* meters = 0;

	List* p;
	for(p = plugins; p; p = p->Cdr)
	{
		TCHAR params[PARAMS_BUFSIZE_MAX];
		TCHAR* param = params;
		Plugin* plugin = (Plugin*)p->Car;
		plugin->GetAvailableParameters(params, PARAMS_BUFSIZE_MAX);

		while(*param)
		{
			List* node = ListCreate();
			node->Car = MeterCreate(plugin->PluginName, param);
			ListAppend(&meters, node);

			param += lstrlen(param) + 1;
		}
	}

	return meters;
}

void DestroyMeters(List* meters)
{
	List* p = meters;
	while(p)
	{
		if(p->Car)
		{
			Meter* meter = (Meter*)p->Car;
			if(meter->PluginName)
			{
				free(meter->PluginName);
				meter->PluginName = 0;
			}
			if(meter->Parameter)
			{
				free(meter->Parameter);
				meter->Parameter = 0;
			}
			free(meter);
			p->Car = 0;
		}
		p = p->Cdr;
	}
}