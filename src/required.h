#pragma once

#include <Windows.h>
#include <Psapi.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "Gdiplus.lib")
#include <CommCtrl.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include <windowsx.h>
#include <ShObjIdl.h>
#include <atlbase.h>
#include <shellapi.h>

#include "resource.h"
#include "WmaskConfig.h"
#include <fstream>
#include <map>
#include <set>
#include <vector>

// WmaskMain HMENU
#define ID_New                    0x1001
#define ID_Setting                0x1002
#define ID_Enable                 0x1003
#define ID_ConfigList             0x1004

// WmaskConfigPanel HMENU
#define ID_Import                 0x2001
#define ID_Export                 0x2002
#define ID_Delete                 0x2003
#define ID_SelectExePath          0x2004
#define ID_SelectDirPath          0x2005
#define ID_SelectPreviewPath      0x2006
#define ID_Cancel                 0x2007
#define ID_Confirm                0x2008
#define ID_DropdownExePath        0x2009

// WmaskTray UID and HMENU
#define ID_WmaskTray              0x3001
#define ID_ShowWmaskMain          0x3002
#define ID_QuitWmaskMain          0x3003
#define ID_WmaskTrayStart         0x3004
// ID_WmaskTrayMenuitem between 0x3004 and 0x4000
#define ID_WmaskTrayEnd           0x4000

// Custom Message WmaskConfigPanel => WmaskMain
#define WM_USER_Confirm           (WM_USER+0x0001)    // wParam: const WmaskConfig* pWC, lParam: PWSTR originalName, return: true for accepted false for rejected
#define WM_USER_Delete            (WM_USER+0x0002)    // wParam: reserved, lParam: PWSTR originalName, return: true for accepted false for rejected
// Custom Message WmaskTray => WmaskMain
#define WM_USER_Tray              (WM_USER+0x0003)
// Custom Message WmaskMain => WmaskImage
#define WM_USER_Update            (WM_USER+0x0004)    // wParam: const WmaskConfig* pWC, lParam: reserved, return: true for success

struct WmaskChild {
	std::wstring configName;
	HWND wmaskImageHwnd; 
	HWND parentHwnd; 
	std::vector<std::wstring> imagePaths; 
	int lastTime; 
	int curIdx;
	Image* curImage; 
	Size parentSize; 
	Rect wmaskImageRect; 
	WmaskChild() :
		configName(L""),
		wmaskImageHwnd(NULL),
		parentHwnd(NULL),
		imagePaths(),
		lastTime(0),
		curIdx(0),
		curImage(NULL),
		parentSize({0, 0}),
		wmaskImageRect({0, 0, 0, 0})
	{}
};

extern HINSTANCE gHINSTANCE;
extern ULONG_PTR gGdiplusToken;
extern GdiplusStartupInput gGdiplusStartupInput;
extern std::wstring gPreviewImagePath; 
extern std::wstring gConfigPath; 
extern std::wstring gInitDirPath; 
extern Color gBackground; 
extern HBRUSH gBackgroundBrush; 
extern HWND gWmaskMainHwnd; 

extern std::vector<std::wstring> gConfigNames;
extern std::map<std::wstring, WmaskConfig> gWmaskConfigs; 
extern std::map<std::wstring, std::vector<WmaskChild*>> gWmaskChilds; 
extern std::map<std::wstring, std::set<HWND>> gHandledHwnds; 
extern std::map<HWND, std::wstring> gHwnd2exepaths;

const int wmaskMainRefreshDuration = 100;  // ms
const int wmaskImageRefreshDuration = 100;  // ms

inline bool _IsValidTopWindow(HWND hwnd) {
	return IsWindow(hwnd) &&
		IsWindowVisible(hwnd) &&
		IsWindowEnabled(hwnd) &&
		!(GetWindowLongPtr(hwnd, GWL_STYLE) & (WS_POPUP | WS_CHILD));
}

// WmaskPreview.h
void RegisterWmaskPreviewClass(); 
HWND CreateWmaskPreviewWindow(HWND parent, int x, int y, int w, int h);

// WmaskConfigPanel.h
void RegisterWmaskConfigPanelClass();
HWND CreateWmaskConfigPanelWindow(HWND parent, const WmaskConfig& wc);

// WmaskImage.h
void RegisterWmaskImageClass();
WmaskChild* CreateWmaskImageWindow(HWND parent, const std::wstring configName);

// WmaskTray.h
void CreateWmaskTray(HWND hwnd);
void DestroyWmaskTray(HWND hwnd);
void PopupWmaskTrayMenu(HWND hwnd);

// WmaskMain.h
void RegisterWmaskMainClass();
HWND CreateWmaskMainWindow();

void OpenConfig();
void SaveConfig();

