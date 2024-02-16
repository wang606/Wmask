#include "WmaskConfigPanel.h"

std::wstring originalName; 
HWND wmaskConfigPanelHwnd; 
HWND importButtonHwnd, exportButtonHwnd, deleteButtonHwnd; 
HWND nameEditHwnd, exepathEditHwnd, dirpathEditHwnd, previewEditHwnd; 
HWND exepathButtonHwnd, dirpathButtonHwnd, previewButtonHwnd; 
HWND sizeComboboxHwnd, horizonComboboxHwnd, verticalComboboxHwnd;
HWND scaleEditHwnd, xshiftEditHwnd, yshiftEditHwnd; 
HWND playComboboxHwnd, durationEditHwnd, fadedurationEditHwnd, opacityTrackHwnd; 

void _loadConfig(const WmaskConfig& wc); 
void _formConfig(WmaskConfig& wc); 
bool exepathEditOnDropdown(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
bool importButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result); 
bool exportButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
bool deleteButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
bool exepathButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
bool dirpathButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
bool previewButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
bool cancelButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
bool confirmButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
bool wmaskConfigPanelOnDestroy(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result); 

LRESULT CALLBACK WmaskConfigPanelWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result; 
	if (exepathEditOnDropdown(hwnd, uMsg, wParam, lParam, result)) return result; 
	if (importButtonOnClicked(hwnd, uMsg, wParam, lParam, result)) return result;
	if (exportButtonOnClicked(hwnd, uMsg, wParam, lParam, result)) return result;
	if (deleteButtonOnClicked(hwnd, uMsg, wParam, lParam, result)) return result;
	if (exepathButtonOnClicked(hwnd, uMsg, wParam, lParam, result)) return result;
	if (dirpathButtonOnClicked(hwnd, uMsg, wParam, lParam, result)) return result;
	if (previewButtonOnClicked(hwnd, uMsg, wParam, lParam, result)) return result;
	if (cancelButtonOnClicked(hwnd, uMsg, wParam, lParam, result)) return result;
	if (confirmButtonOnClicked(hwnd, uMsg, wParam, lParam, result)) return result;
	if (wmaskConfigPanelOnDestroy(hwnd, uMsg, wParam, lParam, result)) return result; 
	return DefWindowProc(hwnd, uMsg, wParam, lParam); 
}

void RegisterWmaskConfigPanelClass() {
	WNDCLASS wc = { 0 }; 
	wc.lpszClassName = L"WmaskConfigPanel"; 
	wc.lpfnWndProc = WmaskConfigPanelWndProc; 
	wc.hInstance = gHINSTANCE; 
	wc.hbrBackground = gBackgroundBrush; 
	wc.hIcon = LoadIcon(gHINSTANCE, MAKEINTRESOURCE(IDI_Favicon)); 
	RegisterClass(&wc); 
}

HWND CreateWmaskConfigPanelWindow(HWND parent, const WmaskConfig& wc) {
	// UI constant
	int width = 500, height = 550; 
	int padding = 10; 
	int rowHeight = 24;
	int importButtonWidth = 180; 
	int exportButtonWidth = 180; 
	int deleteButtonWidth = width - padding * 4 - importButtonWidth - exportButtonWidth; 
	int basicWidth = width - padding * 2; 
	int basicHeight = rowHeight * 5 + padding * 4; 
	int nameLabelWidth = 80; 
	int nameEditWidth = basicWidth - padding * 3 - nameLabelWidth; 
	int exepathLabelWidth = 80; 
	int exepathButtonWidth = 100; 
	int exepathEditWidth = basicWidth - padding * 4 - exepathLabelWidth - exepathButtonWidth; 
	int dirpathLabelWidth = exepathLabelWidth; 
	int dirpathButtonWidth = exepathButtonWidth; 
	int dirpathEditWidth = exepathEditWidth; 
	int previewLabelWidth = exepathLabelWidth; 
	int previewButtonWidth = exepathButtonWidth; 
	int previewEditWidth = exepathEditWidth; 
	int geometryWidth = width - padding * 2; 
	int geometryHeight = rowHeight * 4 + padding * 3; 
	int sizeLabelWidth = 80; 
	int scaleLabelWidth = 80;
	int scaleEditWidth = 80;
	int sizeComboboxWidth = geometryWidth - padding * 5 - sizeLabelWidth - scaleLabelWidth - scaleEditWidth; 
	int horizonLabelWidth = 80; 
	int xshiftLabelWidth = 80;
	int xshiftEditWidth = 80;
	int horizonComboboxWidth = geometryWidth - padding * 5 - horizonLabelWidth - xshiftLabelWidth - xshiftEditWidth; 
	int verticalLabelWidth = 80; 
	int yshiftLabelWidth = 80;
	int yshiftEditWidth = 80;
	int verticalComboboxWidth = geometryWidth - padding * 5 - verticalLabelWidth - yshiftLabelWidth - yshiftEditWidth; 
	int playWidth = width - padding * 2; 
	int playHeight = rowHeight * 4 + padding * 3; 
	int playLabelWidth = 80; 
	int playComboboxWidth = playWidth - padding * 3 - playLabelWidth; 
	int durationLabelWidth = 80; 
	int fadedurationLabelWidth = 100; 
	int durationEditWidth = (playWidth - padding * 5 - durationLabelWidth - fadedurationLabelWidth) / 2; 
	int fadedurationEditWidth = durationEditWidth; 
	int opacityLabelWidth = 80; 
	int opacityTrackWidth = playWidth - padding * 3 - opacityLabelWidth; 
	int cancelButtonWidth = 100; 
	int confirmButtonWidth = width - padding * 3 - cancelButtonWidth; 

	// UI init
	int x = padding, y = padding; 
	wmaskConfigPanelHwnd = CreateWindowEx(NULL, L"WmaskConfigPanel", L"Wmask Config Panel", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, width + 20, height, parent, NULL, gHINSTANCE, NULL); 
	SetWindowLongPtr(wmaskConfigPanelHwnd, GWLP_USERDATA, (LONG_PTR)parent); 
	auto CW = [&x, &y, padding, rowHeight] (PCWSTR controlType, PCWSTR title, int w, int h, int id = 0, DWORD extraStyle = NULL, int nextlineX = 0, int deltaY = 0) -> HWND {
		/*
		* id: HMENU
		* extraStyle: extra styles except `WS_VISIBLE` and `WS_CHILD`
		* nextlineX: if > 0, the next UI element will wrap and start at `nextlineX`
		* deltaY: valid when `nextlineX` > 0. specify the y increment. 
		*/
		HWND hwnd = CreateWindow(controlType, title, WS_VISIBLE | WS_CHILD | extraStyle, x, y, w, h, wmaskConfigPanelHwnd, (HMENU)id, gHINSTANCE, NULL);
		if (nextlineX > 0) {
			x = nextlineX; 
			y += (deltaY > 0) ? deltaY : (rowHeight + padding); 
		}
		else {
			x += w + padding; 
		}
		return hwnd; 
	}; 
	importButtonHwnd = CW(L"BUTTON", L"Import", importButtonWidth, rowHeight, ID_Import); 
	exportButtonHwnd = CW(L"BUTTON", L"Export", exportButtonWidth, rowHeight, ID_Export); 
	deleteButtonHwnd = CW(L"BUTTON", L"Delete", deleteButtonWidth, rowHeight, ID_Delete, NULL, padding); 
	
	CW(L"BUTTON", L"Basic", basicWidth, basicHeight, NULL, BS_GROUPBOX, padding * 2, rowHeight);
	CW(L"STATIC", L"name: ", nameLabelWidth, rowHeight); 
	nameEditHwnd = CW(L"EDIT", NULL, nameEditWidth, rowHeight, NULL, WS_BORDER | ES_AUTOHSCROLL, padding * 2); 
	SendMessage(nameEditHwnd, EM_SETLIMITTEXT, MAX_PATH - 10, 0); 
	CW(L"STATIC", L"exe path: ", exepathLabelWidth, rowHeight); 
	exepathEditHwnd = CW(L"COMBOBOX", NULL, exepathEditWidth, rowHeight, ID_DropdownExePath, CBS_DROPDOWN | CBS_AUTOHSCROLL);
	SendMessage(exepathEditHwnd, EM_SETLIMITTEXT, MAX_PATH - 10, 0);
	exepathButtonHwnd = CW(L"BUTTON", L"select", exepathButtonWidth, rowHeight, ID_SelectExePath, NULL, padding * 2); 
	CW(L"STATIC", L"dir path: ", dirpathLabelWidth, rowHeight); 
	dirpathEditHwnd = CW(L"EDIT", NULL, dirpathEditWidth, rowHeight, NULL, WS_BORDER | ES_AUTOHSCROLL);
	SendMessage(dirpathEditHwnd, EM_SETLIMITTEXT, MAX_PATH - 10, 0);
	dirpathButtonHwnd = CW(L"BUTTON", L"select", dirpathButtonWidth, rowHeight, ID_SelectDirPath, NULL, padding * 2); 
	CW(L"STATIC", L"preview: ", previewLabelWidth, rowHeight); 
	previewEditHwnd = CW(L"EDIT", NULL, previewEditWidth, rowHeight, NULL, WS_BORDER | ES_AUTOHSCROLL);
	SendMessage(previewEditHwnd, EM_SETLIMITTEXT, MAX_PATH - 10, 0);
	previewButtonHwnd = CW(L"BUTTON", L"select", previewButtonWidth, rowHeight, ID_SelectPreviewPath, NULL, padding); 
	
	CW(L"BUTTON", L"Geometry", geometryWidth, geometryHeight, NULL, BS_GROUPBOX, padding * 2, rowHeight); 
	CW(L"STATIC", L"Size: ", sizeLabelWidth, rowHeight); 
	sizeComboboxHwnd = CW(L"COMBOBOX", NULL, sizeComboboxWidth, rowHeight, NULL, CBS_DROPDOWNLIST); 
	ComboBox_AddString(sizeComboboxHwnd, L"Fixed");
	ComboBox_AddString(sizeComboboxHwnd, L"Fit");
	ComboBox_AddString(sizeComboboxHwnd, L"Fill");
	ComboBox_SetCurSel(sizeComboboxHwnd, 0); 
	CW(L"STATIC", L"Scale: ", scaleLabelWidth, rowHeight); 
	scaleEditHwnd = CW(L"EDIT", NULL, scaleEditWidth, rowHeight, NULL, WS_BORDER | ES_NUMBER, padding * 2); 
	CW(L"STATIC", L"Horizon: ", horizonLabelWidth, rowHeight); 
	horizonComboboxHwnd = CW(L"COMBOBOX", NULL, horizonComboboxWidth, rowHeight, NULL, CBS_DROPDOWNLIST);
	ComboBox_AddString(horizonComboboxHwnd, L"Center");
	ComboBox_AddString(horizonComboboxHwnd, L"Left");
	ComboBox_AddString(horizonComboboxHwnd, L"Right");
	ComboBox_SetCurSel(horizonComboboxHwnd, 0); 
	CW(L"STATIC", L"x shift: ", xshiftLabelWidth, rowHeight); 
	xshiftEditHwnd = CW(L"EDIT", NULL, xshiftEditWidth, rowHeight, NULL, WS_BORDER | ES_NUMBER, padding * 2); 
	CW(L"STATIC", L"Vertical: ", verticalLabelWidth, rowHeight);
	verticalComboboxHwnd = CW(L"COMBOBOX", NULL, verticalComboboxWidth, rowHeight, NULL, CBS_DROPDOWNLIST);
	ComboBox_AddString(verticalComboboxHwnd, L"Center");
	ComboBox_AddString(verticalComboboxHwnd, L"Up");
	ComboBox_AddString(verticalComboboxHwnd, L"Down");
	ComboBox_SetCurSel(verticalComboboxHwnd, 0);
	CW(L"STATIC", L"y shift: ", yshiftLabelWidth, rowHeight);
	yshiftEditHwnd = CW(L"EDIT", NULL, yshiftEditWidth, rowHeight, NULL, WS_BORDER | ES_NUMBER, padding);

	CW(L"BUTTON", L"Play", playWidth, playHeight, NULL, BS_GROUPBOX, padding * 2, rowHeight); 
	CW(L"STATIC", L"play mode: ", playLabelWidth, rowHeight); 
	playComboboxHwnd = CW(L"COMBOBOX", NULL, playComboboxWidth, rowHeight, NULL, CBS_DROPDOWNLIST, padding * 2);
	ComboBox_AddString(playComboboxHwnd, L"Repeat");
	ComboBox_AddString(playComboboxHwnd, L"Loop");
	ComboBox_AddString(playComboboxHwnd, L"Shuffle");
	ComboBox_SetCurSel(playComboboxHwnd, 0);
	CW(L"STATIC", L"duration: ", durationLabelWidth, rowHeight); 
	durationEditHwnd = CW(L"EDIT", NULL, durationEditWidth, rowHeight, NULL, WS_BORDER | ES_NUMBER); 
	CW(L"STATIC", L"fade duration: ", fadedurationLabelWidth, rowHeight); 
	fadedurationEditHwnd = CW(L"EDIT", NULL, fadedurationEditWidth, rowHeight, NULL, WS_BORDER | ES_NUMBER, padding * 2); 
	CW(L"STATIC", L"Opacity: ", opacityLabelWidth, rowHeight); 
	opacityTrackHwnd = CW(L"msctls_trackbar32", NULL, opacityTrackWidth, rowHeight, NULL, NULL, padding, rowHeight + padding * 2);
	SendMessage(opacityTrackHwnd, TBM_SETRANGEMIN, FALSE, 0); 
	SendMessage(opacityTrackHwnd, TBM_SETRANGEMAX, FALSE, 255); 

	CW(L"BUTTON", L"Cancel", cancelButtonWidth, rowHeight, ID_Cancel); 
	CW(L"BUTTON", L"Confirm", confirmButtonWidth, rowHeight, ID_Confirm, NULL, padding); 

	// fill WmaskConfig
	_loadConfig(wc); 

	// originalName
	originalName = wc.name; 

	// active status
	if (wc.name != L"" && wc.active) {
		EnableWindow(importButtonHwnd, FALSE); 
		EnableWindow(deleteButtonHwnd, FALSE); 
		EnableWindow(nameEditHwnd, FALSE); 
		EnableWindow(exepathEditHwnd, FALSE); 
		EnableWindow(dirpathEditHwnd, FALSE); 
		EnableWindow(exepathButtonHwnd, FALSE); 
		EnableWindow(dirpathButtonHwnd, FALSE); 
	}

	EnableWindow(parent, FALSE); 
	return wmaskConfigPanelHwnd; 
}

void _loadConfig(const WmaskConfig& wc) {
	Edit_SetText(nameEditHwnd, wc.name.c_str());
	ComboBox_SetText(exepathEditHwnd, wc.exepath.c_str());
	Edit_SetText(dirpathEditHwnd, wc.dirpath.c_str());
	Edit_SetText(previewEditHwnd, wc.preview.c_str());
	ComboBox_SetCurSel(sizeComboboxHwnd, wc.size);
	Edit_SetText(scaleEditHwnd, std::to_wstring(wc.scale).c_str());
	ComboBox_SetCurSel(horizonComboboxHwnd, wc.horizon);
	Edit_SetText(xshiftEditHwnd, std::to_wstring(wc.xshift).c_str());
	ComboBox_SetCurSel(verticalComboboxHwnd, wc.vertical);
	Edit_SetText(yshiftEditHwnd, std::to_wstring(wc.yshift).c_str());
	ComboBox_SetCurSel(playComboboxHwnd, wc.playmode);
	Edit_SetText(durationEditHwnd, std::to_wstring(wc.duration).c_str());
	Edit_SetText(fadedurationEditHwnd, std::to_wstring(wc.fadeduration).c_str());
	SendMessage(opacityTrackHwnd, TBM_SETPOS, TRUE, wc.opacity);
}

void _formConfig(WmaskConfig& wc) {
	PWSTR text = (PWSTR)malloc(sizeof(WCHAR) * MAX_PATH); 
	Edit_GetText(nameEditHwnd, text, MAX_PATH); 
	wc.name = text; 
	ComboBox_GetText(exepathEditHwnd, text, MAX_PATH); 
	wc.exepath = text; 
	Edit_GetText(dirpathEditHwnd, text, MAX_PATH); 
	wc.dirpath = text; 
	Edit_GetText(previewEditHwnd, text, MAX_PATH); 
	wc.preview = text; 
	wc.size = (WmaskConfig::GeometrySize)ComboBox_GetCurSel(sizeComboboxHwnd); 
	Edit_GetText(scaleEditHwnd, text, MAX_PATH); 
	wc.scale = std::stoi(text); 
	wc.horizon = (WmaskConfig::GeometryHorizon)ComboBox_GetCurSel(horizonComboboxHwnd); 
	Edit_GetText(xshiftEditHwnd, text, MAX_PATH); 
	wc.xshift = std::stoi(text); 
	wc.vertical = (WmaskConfig::GeometryVertical)ComboBox_GetCurSel(verticalComboboxHwnd);
	Edit_GetText(yshiftEditHwnd, text, MAX_PATH);
	wc.yshift = std::stoi(text); 
	wc.playmode = (WmaskConfig::PlayMode)ComboBox_GetCurSel(playComboboxHwnd); 
	Edit_GetText(durationEditHwnd, text, MAX_PATH); 
	wc.duration = std::stoi(text); 
	Edit_GetText(fadedurationEditHwnd, text, MAX_PATH); 
	wc.fadeduration = std::stoi(text); 
	wc.opacity = (int)SendMessage(opacityTrackHwnd, TBM_GETPOS, 0, 0); 
	free(text); 
}

bool exepathEditOnDropdown(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_COMMAND && LOWORD(wParam) == ID_DropdownExePath && HIWORD(wParam) == CBN_DROPDOWN) {
		int remainNum; 
		do
			remainNum = SendMessage(exepathEditHwnd, CB_DELETESTRING, 0, 0);
		while (remainNum > 0); 
		std::set<std::wstring> exepaths; 
		for (auto i : gHwnd2exepaths)
			if (_IsValidTopWindow(i.first))
				exepaths.insert(i.second); 
		for (std::wstring exepath : exepaths)
			ComboBox_AddString(exepathEditHwnd, exepath.c_str()); 
		result = TRUE; 
		return true; 
	}
	else return false; 
}

bool importButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_COMMAND && LOWORD(wParam) == ID_Import && HIWORD(wParam) == BN_CLICKED) {
		CComPtr<IFileOpenDialog> pFileOpen; 
		if (SUCCEEDED(pFileOpen.CoCreateInstance(__uuidof(FileOpenDialog)))) {
			pFileOpen->SetTitle(L"Import Config file");
			COMDLG_FILTERSPEC filter = { L"JSON", L"*.json" };
			pFileOpen->SetFileTypes(1, &filter);
			if (SUCCEEDED(pFileOpen->Show(NULL))) {
				CComPtr<IShellItem> pItem;
				if (SUCCEEDED(pFileOpen->GetResult(&pItem))) {
					PWSTR pszFilePath;
					if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
						try {
							std::ifstream config(pszFilePath);
							json configJson = json::parse(config);
							config.close(); 
							_loadConfig(configJson); 
						}
						catch (...) {
							MessageBox(NULL, L"invalid config file!", L"Warn", MB_OK); 
						}
						CoTaskMemFree(pszFilePath);
					}
				}
			}
		}
		result = TRUE;
		return true;
	}
	else return false; 
}

bool exportButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_COMMAND && LOWORD(wParam) == ID_Export && HIWORD(wParam) == BN_CLICKED) {
		CComPtr<IFileSaveDialog> pFileSave;
		if (SUCCEEDED(pFileSave.CoCreateInstance(__uuidof(FileSaveDialog)))) {
			pFileSave->SetTitle(L"Export Config file");
			COMDLG_FILTERSPEC filter = { L"JSON", L"*.json" };
			pFileSave->SetFileTypes(1, &filter);
			if (SUCCEEDED(pFileSave->Show(NULL))) {
				CComPtr<IShellItem> pItem;
				if (SUCCEEDED(pFileSave->GetResult(&pItem))) {
					PWSTR pszFilePath;
					if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
						try {
							std::ofstream config(pszFilePath);
							WmaskConfig wc; 
							_formConfig(wc); 
							json configJson = wc;
							config << configJson; 
							config.close(); 
						}
						catch (...) {
							MessageBox(NULL, L"invalid config file!", L"Warn", MB_OK);
						}
						CoTaskMemFree(pszFilePath);
					}
				}
			}
		}
		result = TRUE;
		return true;
	}
	else return false;
}

bool deleteButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_COMMAND && LOWORD(wParam) == ID_Delete && HIWORD(wParam) == BN_CLICKED) {
		if (MessageBox(NULL, L"sure to delete this config?", L"Warn", MB_ICONWARNING | MB_OKCANCEL) == IDOK) {
			HWND parent = (HWND)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			if (SendMessage(parent, WM_USER_Delete, NULL, (LPARAM)originalName.c_str()))
				DestroyWindow(hwnd); 
		}
		result = TRUE;
		return true;
	}
	else return false;
}

bool exepathButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_COMMAND && LOWORD(wParam) == ID_SelectExePath && HIWORD(wParam) == BN_CLICKED) {
		CComPtr<IFileOpenDialog> pFileOpen;
		if (SUCCEEDED(pFileOpen.CoCreateInstance(__uuidof(FileOpenDialog)))) {
			pFileOpen->SetTitle(L"Select Exe Path");
			COMDLG_FILTERSPEC filter = { L"EXE", L"*.exe" };
			pFileOpen->SetFileTypes(1, &filter);
			if (SUCCEEDED(pFileOpen->Show(NULL))) {
				CComPtr<IShellItem> pItem;
				if (SUCCEEDED(pFileOpen->GetResult(&pItem))) {
					PWSTR pszFilePath;
					if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
						Edit_SetText(exepathEditHwnd, pszFilePath); 
						CoTaskMemFree(pszFilePath);
					}
				}
			}
		}
		result = TRUE;
		return true;
	}
	else return false;
}

bool dirpathButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_COMMAND && LOWORD(wParam) == ID_SelectDirPath && HIWORD(wParam) == BN_CLICKED) {
		CComPtr<IFileOpenDialog> pFileOpen;
		if (SUCCEEDED(pFileOpen.CoCreateInstance(__uuidof(FileOpenDialog)))) {
			pFileOpen->SetTitle(L"Select Dir Path");
			pFileOpen->SetOptions(FOS_PICKFOLDERS); 
			if (SUCCEEDED(pFileOpen->Show(NULL))) {
				CComPtr<IShellItem> pItem;
				if (SUCCEEDED(pFileOpen->GetResult(&pItem))) {
					PWSTR pszFilePath;
					if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
						Edit_SetText(dirpathEditHwnd, pszFilePath); 
						CoTaskMemFree(pszFilePath);
					}
				}
			}
		}
		result = TRUE;
		return true;
	}
	else return false;
}

bool previewButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_COMMAND && LOWORD(wParam) == ID_SelectPreviewPath && HIWORD(wParam) == BN_CLICKED) {
		CComPtr<IFileOpenDialog> pFileOpen;
		if (SUCCEEDED(pFileOpen.CoCreateInstance(__uuidof(FileOpenDialog)))) {
			pFileOpen->SetTitle(L"Select Preview Picture");
			COMDLG_FILTERSPEC filter = { L"Image", L"*.bmp;*.icon;*.jpeg;*.jpg;*.exif;*.png;*.tiff;*.wmf;*.emf" };
			pFileOpen->SetFileTypes(1, &filter);
			if (SUCCEEDED(pFileOpen->Show(NULL))) {
				CComPtr<IShellItem> pItem;
				if (SUCCEEDED(pFileOpen->GetResult(&pItem))) {
					PWSTR pszFilePath;
					if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
						Edit_SetText(previewEditHwnd, pszFilePath); 
						CoTaskMemFree(pszFilePath);
					}
				}
			}
		}
		result = TRUE;
		return true;
	}
	else return false;
}

bool cancelButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_COMMAND && LOWORD(wParam) == ID_Cancel && HIWORD(wParam) == BN_CLICKED) {
		DestroyWindow(wmaskConfigPanelHwnd); 
		result = TRUE;
		return true;
	}
	else return false;
}

bool confirmButtonOnClicked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_COMMAND && LOWORD(wParam) == ID_Confirm && HIWORD(wParam) == BN_CLICKED) {
		WmaskConfig wc; 
		_formConfig(wc); 
		HWND parent = (HWND)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (SendMessage(parent, WM_USER_Confirm, (WPARAM)&wc, (LPARAM)originalName.c_str()))
			DestroyWindow(hwnd); 
		result = TRUE;
		return true;
	}
	else return false;
}

bool wmaskConfigPanelOnDestroy(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_DESTROY) {
		HWND parent = (HWND)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		EnableWindow(parent, TRUE);
		SetForegroundWindow(parent);
		result = 0;
		return true;
	}
	else return false; 
}