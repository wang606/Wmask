#include "required.h"

HWND configListHwnd, previewHwnd, enableButtonHwnd; 

bool wmaskMainOnTimeout(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
bool configListOnSelChange(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result); 
bool newButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
bool settingButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
bool enableButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
bool wmaskMainConfirmSlot(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
bool wmaskMainDeleteSlot(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
bool wmaskMainTraySlot(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
bool wmaskMainOnClose(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
bool wmaskMainOnDestroy(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);

LRESULT CALLBACK WmaskMainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result; 
	if (wmaskMainOnTimeout(hwnd, uMsg, wParam, lParam, result)) return result; 
	if (configListOnSelChange(hwnd, uMsg, wParam, lParam, result)) return result; 
	if (newButtonOnClicked(hwnd, uMsg, wParam, lParam, result)) return result;
	if (settingButtonOnClicked(hwnd, uMsg, wParam, lParam, result)) return result;
	if (enableButtonOnClicked(hwnd, uMsg, wParam, lParam, result)) return result;
	if (wmaskMainConfirmSlot(hwnd, uMsg, wParam, lParam, result)) return result;
	if (wmaskMainDeleteSlot(hwnd, uMsg, wParam, lParam, result)) return result;
	if (wmaskMainTraySlot(hwnd, uMsg, wParam, lParam, result)) return result; 
	if (wmaskMainOnClose(hwnd, uMsg, wParam, lParam, result)) return result; 
	if (wmaskMainOnDestroy(hwnd, uMsg, wParam, lParam, result)) return result;
	return DefWindowProc(hwnd, uMsg, wParam, lParam); 
}

void RegisterWmaskMainClass() {
	WNDCLASS wc = { 0 }; 
	wc.lpszClassName = L"WmaskMain"; 
	wc.lpfnWndProc = WmaskMainWndProc; 
	wc.hInstance = gHINSTANCE; 
	wc.hbrBackground = gBackgroundBrush; 
	wc.hIcon = LoadIcon(gHINSTANCE, MAKEINTRESOURCE(IDI_Favicon)); 
	RegisterClass(&wc); 
}

HWND CreateWmaskMainWindow() {
	// UI init
	HWND wmaskMainHwnd = CreateWindowEx(NULL, L"WmaskMain", L"Wmask win32 v1.1.0", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, gHINSTANCE, NULL); 
	configListHwnd = CreateWindow(L"LISTBOX", NULL, WS_VISIBLE | WS_CHILD | LBS_STANDARD | LBS_DISABLENOSCROLL, 10, 10, 320, 430, wmaskMainHwnd, (HMENU)ID_ConfigList, gHINSTANCE, NULL);
	CreateWindow(L"BUTTON", L"New", WS_VISIBLE | WS_CHILD, 348, 430 - 24 - 4, 80, 24, wmaskMainHwnd, (HMENU)ID_New, gHINSTANCE, NULL);
	CreateWindow(L"BUTTON", L"Setting", WS_VISIBLE | WS_CHILD, 348 + 80 + 10, 430 - 24 - 4, 80, 24, wmaskMainHwnd, (HMENU)ID_Setting, gHINSTANCE, NULL);
	enableButtonHwnd = CreateWindow(L"BUTTON", L"Enable", WS_VISIBLE | WS_CHILD, 348 + 80 + 10 + 80 + 10, 430 - 24 - 4, 80, 24, wmaskMainHwnd, (HMENU)ID_Enable, gHINSTANCE, NULL);
	previewHwnd = CreateWmaskPreviewWindow(wmaskMainHwnd, 348, 10, 260, 380); 
	// fill gWmaskConfigs
	for (auto i : gWmaskConfigs)
		ListBox_AddString(configListHwnd, i.second.name.c_str());
	SetTimer(wmaskMainHwnd, 0, wmaskMainRefreshDuration, NULL); 
	return wmaskMainHwnd; 
}

BOOL CALLBACK _EnumWindowsCallback(HWND hwnd, LPARAM) {
	if (gHwnd2exepaths.contains(hwnd)) return TRUE; 
	if (IsWindow(hwnd)) {
		PWSTR pText = (PWSTR)malloc(sizeof(WCHAR) * MAX_PATH); 
		DWORD pid = 0; 
		HANDLE handle = NULL; 
		HMODULE hmod = NULL; 
		DWORD cbNeeded; 
		GetWindowThreadProcessId(hwnd, &pid); 
		if (pid != 0) {
			handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pid); 
			if (handle != NULL && EnumProcessModules(handle, &hmod, sizeof(hmod), &cbNeeded)) {
				GetModuleFileNameEx(handle, hmod, pText, MAX_PATH); 
				gHwnd2exepaths[hwnd] = pText; 
			}
		}
		free(pText); 
	}
	return TRUE; 
}

bool wmaskMainOnTimeout(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_TIMER) {
		for (auto i = gHwnd2exepaths.begin(); i != gHwnd2exepaths.end(); )
			if (!IsWindow((*i).first))
				i = gHwnd2exepaths.erase(i);
			else
				++i; 
		EnumWindows(_EnumWindowsCallback, 0); 
		// 跟踪创建 wmaskImage
		for (auto i : gWmaskConfigs) {
			if (!i.second.active) continue; 
			for (auto j : gHwnd2exepaths)
				if (j.second == i.second.exepath && !gHandledHwnds[i.first].contains(j.first) && _IsValidTopWindow(j.first)) {
					gWmaskChilds[i.first].push_back(CreateWmaskImageWindow(j.first, i.first));
					gHandledHwnds[i.first].insert(j.first); 
				}
		}
		// 跟踪销毁 wmaskImage
		for (auto i : gWmaskConfigs) {
			if (!i.second.active) continue; 
			for (auto wmaskChild = gWmaskChilds[i.first].begin(); wmaskChild != gWmaskChilds[i.first].end(); ) {
				if (!IsWindow((*wmaskChild)->wmaskImageHwnd) || !_IsValidTopWindow((*wmaskChild)->parentHwnd)) {
					DestroyWindow((*wmaskChild)->wmaskImageHwnd);
					if ((*wmaskChild)->curImage != NULL) delete (*wmaskChild)->curImage;
					gHandledHwnds[i.first].erase((*wmaskChild)->parentHwnd);
					delete (*wmaskChild);
					wmaskChild = gWmaskChilds[i.first].erase(wmaskChild);
				}
				else ++wmaskChild; 
			}
		}
		result = TRUE; 
		return true; 
	}
	else return false; 
}

bool configListOnSelChange(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_COMMAND && LOWORD(wParam) == ID_ConfigList && HIWORD(wParam) == LBN_SELCHANGE) {
		int cur = (int)SendMessage(configListHwnd, LB_GETCURSEL, 0, 0); 
		if (cur >= 0) {
			PWSTR text = (PWSTR)malloc(sizeof(WCHAR) * MAX_PATH);
			ListBox_GetText(configListHwnd, cur, text);
			if (gWmaskConfigs.contains(text)) {
				gPreviewImagePath = gWmaskConfigs[text].preview;
				InvalidateRect(previewHwnd, NULL, FALSE);
				Button_SetText(enableButtonHwnd, gWmaskConfigs[text].active ? L"Disable" : L"Enable");
			}
			else MessageBox(NULL, L"config doesn't exist!", L"Error", MB_ICONERROR | MB_OK); 
			free(text); 
		}
		result = TRUE;
		return true;
	}
	else return false; 
}

bool newButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_COMMAND && LOWORD(wParam) == ID_New && HIWORD(wParam) == BN_CLICKED) {
		CreateWmaskConfigPanelWindow(hwnd, WmaskConfig()); 
		result = TRUE; 
		return true; 
	}
	else return false;
}

bool settingButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_COMMAND && LOWORD(wParam) == ID_Setting && HIWORD(wParam) == BN_CLICKED) {
		int cur = ListBox_GetCurSel(configListHwnd); 
		if (cur >= 0) {
			PWSTR text = (PWSTR)malloc(sizeof(WCHAR) * MAX_PATH);
			ListBox_GetText(configListHwnd, cur, text);
			if (gWmaskConfigs.contains(text))
				CreateWmaskConfigPanelWindow(hwnd, gWmaskConfigs[text]);
			else
				MessageBox(NULL, L"config doesn't exist!", L"Error", MB_ICONERROR | MB_OK);
			free(text);
		}
		result = TRUE; 
		return true; 
	}
	else return false;
}

bool enableButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_COMMAND && LOWORD(wParam) == ID_Enable && HIWORD(wParam) == BN_CLICKED) {
		int cur = ListBox_GetCurSel(configListHwnd); 
		if (cur >= 0) {
			PWSTR text = (PWSTR)malloc(sizeof(WCHAR) * MAX_PATH);
			ListBox_GetText(configListHwnd, cur, text); 
			if (gWmaskConfigs.contains(text)) {
				gWmaskConfigs[text].active ^= true;
				Button_SetText(enableButtonHwnd, gWmaskConfigs[text].active ? L"Disable" : L"Enable"); 
				if (gWmaskConfigs[text].active) {
					gWmaskChilds[text] = std::vector<WmaskChild*>();
					gHandledHwnds[text] = std::set<HWND>();
				} 
				else {
					for (WmaskChild* wmaskChild : gWmaskChilds[text]) {
						DestroyWindow(wmaskChild->wmaskImageHwnd); 
						if (wmaskChild->curImage != NULL) delete wmaskChild->curImage; 
					}
					gWmaskChilds.erase(text); 
					gHandledHwnds.erase(text); 
				}
				free(text);
			}
			else MessageBox(NULL, L"config doesn't exist!", L"Error", MB_ICONERROR | MB_OK);
		}
		SaveConfig();
		result = TRUE; 
		return true;
	}
	else return false;
}

bool wmaskMainConfirmSlot(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_USER_Confirm) {
		std::wstring originalName = (PWSTR)lParam; 
		WmaskConfig* pWC = (WmaskConfig*)wParam;
		if (pWC->name == L"") {
			MessageBox(NULL, L"name shouldn't be empty!", L"Warn", MB_ICONWARNING | MB_OK);
			result = FALSE;
			return true; 
		}
		if (pWC->name != originalName && gWmaskConfigs.contains(pWC->name)) {
			MessageBox(NULL, L"name already exists!", L"Warn", MB_ICONWARNING | MB_OK);
			result = FALSE;
			return true; 
		}
		if (originalName == L"") {
			// new config
			gWmaskConfigs[pWC->name] = pWC;
			ListBox_AddString(configListHwnd, pWC->name.c_str());
			result = TRUE; 
			return true; 
		}
		else {
			// change config
			pWC->active = gWmaskConfigs[originalName].active; // keep active status
			if (pWC->name == originalName) {
				gWmaskConfigs[originalName] = pWC;
				for (WmaskChild* wmaskChild : gWmaskChilds[originalName])
					SendMessage(wmaskChild->wmaskImageHwnd, WM_USER_Update, (WPARAM)&gWmaskConfigs[originalName], 0); 
			}
			else {
				gWmaskConfigs[pWC->name] = pWC; 
				gWmaskConfigs.erase(originalName);
				ListBox_AddString(configListHwnd, pWC->name.c_str());
				int pos = ListBox_FindString(configListHwnd, -1, originalName.c_str()); 
				ListBox_DeleteString(configListHwnd, pos); 
			}
			result = TRUE; 
			return true; 
		}
	}
	else return false; 
}

bool wmaskMainDeleteSlot(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_USER_Delete) {
		std::wstring originalName = (PWSTR)lParam;
		if (originalName == L"") {
			result = FALSE; 
			return true; 
		}
		gWmaskConfigs.erase(originalName); 
		int pos = ListBox_FindString(configListHwnd, -1, originalName.c_str()); 
		ListBox_DeleteString(configListHwnd, pos); 
		result = TRUE; 
		return true; 
	}
	else return false; 
}

bool wmaskMainTraySlot(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_USER_Tray) {
		switch (lParam) {
		case WM_LBUTTONDBLCLK:
			ShowWindow(hwnd, SW_RESTORE);
			SetForegroundWindow(hwnd);
			break; 
		case WM_RBUTTONUP:
			SetForegroundWindow(hwnd); 
			PopupWmaskTrayMenu(hwnd); 
			break; 
		}
		result = TRUE;
		return true;
	}
	else if (uMsg == WM_COMMAND && LOWORD(wParam) > ID_WmaskTrayStart && LOWORD(wParam) < ID_WmaskTrayEnd) {
		std::wstring text = gConfigNames[(UINT)LOWORD(wParam) - ID_WmaskTrayStart - 1]; 
		// TODO keep below code as same as `enableButtonOnClicked`
		gWmaskConfigs[text].active ^= true;
		Button_SetText(enableButtonHwnd, gWmaskConfigs[text].active ? L"Disable" : L"Enable");
		if (gWmaskConfigs[text].active) {
			gWmaskChilds[text] = std::vector<WmaskChild*>();
			gHandledHwnds[text] = std::set<HWND>();
		}
		else {
			for (WmaskChild* wmaskChild : gWmaskChilds[text]) {
				DestroyWindow(wmaskChild->wmaskImageHwnd);
				if (wmaskChild->curImage != NULL) delete wmaskChild->curImage;
			}
			gWmaskChilds.erase(text);
			gHandledHwnds.erase(text);
		}
		SaveConfig();
		result = TRUE; 
		return true; 
	}
	else if (uMsg == WM_COMMAND && LOWORD(wParam) == ID_ShowWmaskMain) {
		ShowWindow(hwnd, SW_RESTORE);
		result = TRUE; 
		return true; 
	}
	else if (uMsg == WM_COMMAND && LOWORD(wParam) == ID_QuitWmaskMain) {
		DestroyWmaskTray(hwnd);
		DestroyWindow(hwnd); 
		result = TRUE; 
		return true; 
	}
	else return false; 
}

bool wmaskMainOnClose(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_CLOSE) {
		ShowWindow(hwnd, SW_HIDE); 
		SaveConfig(); 
		result = 0;
		return true;
	}
	else return false; 
}

bool wmaskMainOnDestroy(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_DESTROY) {
		PostQuitMessage(0); 
		result = 0;
		return true;
	}
	else return false;
}

void OpenConfig() {
	try {
		std::ifstream config(gConfigPath);
		if (config.good()) {
			json configJson = json::parse(config);
			config.close();
			for (WmaskConfig w : configJson)
				gWmaskConfigs[w.name] = w;
		}
	}
	catch (...) {
		MessageBox(NULL, L"invalid config file!", L"Warn", MB_ICONWARNING | MB_OK);
	}
}

void SaveConfig() {
	try {
		std::vector<WmaskConfig> ws;
		for (auto i : gWmaskConfigs)
			ws.push_back(i.second);
		json configJson = ws;
		std::ofstream config(gConfigPath);
		config << configJson;
		config.close();
	}
	catch (...) {
		MessageBox(NULL, L"failed to save config!", L"Warn", MB_ICONWARNING | MB_OK);
	}
}