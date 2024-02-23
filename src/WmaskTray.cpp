#include "WmaskTray.h"

void CreateWmaskTray(HWND hwnd) {
	NOTIFYICONDATA nid; 
	nid.cbSize = sizeof(NOTIFYICONDATA); 
	nid.hWnd = hwnd; 
	nid.uID = ID_WmaskTray; 
	nid.uVersion = NOTIFYICON_VERSION; 
	nid.uCallbackMessage = WM_USER_Tray; 
	nid.hIcon = LoadIcon(gHINSTANCE, MAKEINTRESOURCE(IDI_Favicon)); 
	wcscpy_s(nid.szTip, L"Wmask"); 
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; 
	Shell_NotifyIcon(NIM_ADD, &nid); 
}

void DestroyWmaskTray(HWND hwnd) {
	NOTIFYICONDATA nid; 
	nid.cbSize = sizeof(NOTIFYICONDATA); 
	nid.hWnd = hwnd; 
	nid.uID = ID_WmaskTray; 
	Shell_NotifyIcon(NIM_DELETE, &nid); 
}

void PopupWmaskTrayMenu(HWND hwnd) {
	POINT cursor; 
	GetCursorPos(&cursor); 
	HMENU hMenu = CreatePopupMenu(); 
	UINT_PTR uid = ID_WmaskTrayStart + 1; 
	gConfigNames.clear(); 
	for (auto i : gWmaskConfigs) {
		gConfigNames.push_back(i.first); 
		UINT uFlags = i.second.active ? MF_STRING | MF_CHECKED : MF_STRING; 
		AppendMenu(hMenu, uFlags, uid, i.second.active ? (i.first + L" (" + std::to_wstring(gWmaskImages[i.first].size()) + L")").c_str() : i.first.c_str());
		uid += 1; 
	}
	AppendMenu(hMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hMenu, MF_STRING, ID_ShowWmaskMain, L"Show Wmask"); 
	AppendMenu(hMenu, MF_STRING, ID_QuitWmaskMain, L"Quit Wmask"); 
	SetMenuDefaultItem(hMenu, ID_ShowWmaskMain, false); 
	TrackPopupMenu(hMenu, TPM_LEFTALIGN, cursor.x, cursor.y, 0, hwnd, NULL); 
}
