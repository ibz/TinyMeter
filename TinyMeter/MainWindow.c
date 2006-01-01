#include <windows.h>
#include "List.h"
#include "MainWindow.h"
#include "Meter.h"
#include "Plugin.h"

#define VALUE_BUFSIZE_MAX 128

#define IDI_TRAY 0
#define WM_SHELLNOTIFY (WM_USER + 1)
#define ID_TIMER 512

#define CLASS_NAME TEXT("TinyMeterWindowClas")
#define LABEL_CLASS_NAME TEXT("Static")
#define TITLE TEXT("TinyMeter")

#define WIDTH 160
#define LABEL_WIDTH WIDTH
#define LABEL_HEIGHT 20

static BOOL Visible = TRUE;

static List* Plugins;
static List* Meters;
static List* Labels;

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	case WM_CREATE:
		{
			List* p;
			int i;
			NOTIFYICONDATA notifyIconData;
			notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
			notifyIconData.hWnd = hWnd;
			notifyIconData.uID = IDI_TRAY;
			notifyIconData.uFlags = NIF_ICON + NIF_MESSAGE + NIF_TIP;
			notifyIconData.uCallbackMessage = WM_SHELLNOTIFY;
			lstrcpy(notifyIconData.szTip, TITLE);
			notifyIconData.hIcon = LoadIcon(NULL, IDI_WINLOGO);
			Shell_NotifyIcon(NIM_ADD, &notifyIconData);

			Labels = 0;
			i = 0;
			for(p = Meters; p; p = p->Cdr, i++)
			{
				RECT rect;
				HWND hWndLabel;
				List* node;

				hWndLabel = CreateWindowEx(WS_EX_CLIENTEDGE, LABEL_CLASS_NAME, NULL, SS_LEFTNOWORDWRAP + WS_VISIBLE + WS_CHILD, 0, i * LABEL_HEIGHT, LABEL_WIDTH, LABEL_HEIGHT, hWnd, 0, NULL, 0);

				node = ListCreate();
				node->Car = hWndLabel;
				ListAppend(&Labels, node);

				GetWindowRect(hWnd, &rect);
				rect.bottom += LABEL_HEIGHT;
				SetWindowPos(hWnd, HWND_TOPMOST, rect.left, rect.top, rect.right, rect.bottom, 0);
			}

			SetTimer(hWnd, ID_TIMER, 1000, NULL);
			break;
		}
	case WM_SHELLNOTIFY:
		{
			if(wParam == IDI_TRAY)
			{
				if(lParam == WM_LBUTTONDOWN)
				{
					ShowWindow(hWnd, Visible ? SW_HIDE : SW_SHOW);
					Visible = !Visible;
				}
			}
			break;
		}
	case WM_TIMER:
		{
			if(wParam == ID_TIMER)
			{
				List* m = Meters;
				List* l = Labels;
				while(m)
				{
					Meter* meter = (Meter*)m->Car;
					Plugin* plugin;
					TCHAR buf[VALUE_BUFSIZE_MAX];

					List* p = Plugins;
					while(p && lstrcmp(((Plugin*)p->Car)->PluginName, meter->PluginName))
					{
						p = p->Cdr;
					}
					plugin = (Plugin*)p->Car;

					plugin->GetCurrentValue(meter->Parameter, buf, VALUE_BUFSIZE_MAX);

					SetWindowText((HWND)l->Car, buf);

					m = m->Cdr;
					l = l->Cdr;
				}
			}
			break;
		}
	default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return 0;
}

static HWND MainWindowCreate()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wclass;
	wclass.cbSize = sizeof(WNDCLASSEX);
	wclass.style = CS_HREDRAW + CS_VREDRAW;
	wclass.lpfnWndProc = WndProc;
	wclass.cbClsExtra = 0;
	wclass.cbWndExtra = 0;
	wclass.hInstance = hInstance;
	wclass.hbrBackground = GetSysColorBrush(COLOR_BTNFACE + 1);
	wclass.lpszMenuName = NULL;
	wclass.lpszClassName = CLASS_NAME;
	wclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wclass.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&wclass);

	return CreateWindowEx(0L, CLASS_NAME, TITLE, WS_SYSMENU | WS_VISIBLE, 0, 0, WIDTH, 0, HWND_DESKTOP, NULL, hInstance, NULL);
}

void MainWindowShow(List* plugins, List* meters)
{
	HWND hwnd;
	MSG msg;

	Plugins = plugins;
	Meters = meters;

	hwnd = MainWindowCreate();
	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}