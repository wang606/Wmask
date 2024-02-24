#include "required.h"

HINSTANCE gHINSTANCE;
ULONG_PTR gGdiplusToken;
GdiplusStartupInput gGdiplusStartupInput;
std::wstring gPreviewImagePath; 
std::wstring gConfigPath; 
std::wstring gInitDirPath; 
Color gBackground; 
HBRUSH gBackgroundBrush;
HWND gWmaskMainHwnd; 

std::vector<std::wstring> gConfigNames;
std::map<std::wstring, WmaskConfig> gWmaskConfigs;
std::map<std::wstring, std::vector<WmaskChild*>> gWmaskChilds;
std::map<std::wstring, std::set<HWND>> gHandledHwnds;
std::map<HWND, std::wstring> gHwnd2exepaths;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
	HANDLE hMutex = CreateMutex(NULL, FALSE, L"The only Wmask instance"); 
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		CloseHandle(hMutex); 
		return 0; 
	}
	gHINSTANCE = hInstance;
	GdiplusStartup(&gGdiplusToken, &gGdiplusStartupInput, NULL);
	gPreviewImagePath = L"./cover.png"; 
	gConfigPath = L"./.wmask"; 
	gInitDirPath = L""; 
	gBackground = Color(240, 240, 240); 
	gBackgroundBrush = CreateSolidBrush(RGB(gBackground.GetR(), gBackground.GetG(), gBackground.GetB()));
	if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE))) {
		MessageBox(NULL, L"failed to init COM!", L"Error", MB_ICONERROR | MB_OK); 
		return -1; 
	}

	OpenConfig(); 
	RegisterWmaskConfigPanelClass(); 
	RegisterWmaskImageClass(); 
	RegisterWmaskPreviewClass(); 
	RegisterWmaskMainClass();

	srand(time(NULL)); 
	gWmaskMainHwnd = CreateWmaskMainWindow();
	ShowWindow(gWmaskMainHwnd, SW_HIDE);
	CreateWmaskTray(gWmaskMainHwnd);

	MSG msg; 
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg); 
		DispatchMessage(&msg); 
	}

	SaveConfig(); 
	CoUninitialize(); 
	GdiplusShutdown(gGdiplusToken);
	return msg.wParam; 
}