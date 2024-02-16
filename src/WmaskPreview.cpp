#include "WmaskPreview.h"

void WmaskPreviewOnPaint(HWND hwnd) {
	Image image(gPreviewImagePath.c_str());
	if (image.GetLastStatus() != Status::Ok) {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		Graphics graphics(hdc);
		graphics.Clear(gBackground);
		EndPaint(hwnd, &ps);
		return; 
	}
	try {
		RECT hwndRect;
		GetClientRect(hwnd, &hwndRect);
		int x, y, w, h;
		if (image.GetWidth() * hwndRect.bottom > hwndRect.right * image.GetHeight()) {
			// image width should scaled to hwnd width
			w = hwndRect.right;
			h = int((double)image.GetHeight() * hwndRect.right / image.GetWidth());
			x = 0;
			y = (hwndRect.bottom - h) / 2;
		}
		else {
			h = hwndRect.bottom;
			w = int((double)image.GetWidth() * hwndRect.bottom / image.GetHeight());
			x = (hwndRect.right - w) / 2;
			y = 0;
		}
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		Graphics graphics(hdc);
		graphics.Clear(gBackground);
		graphics.DrawImage(&image, x, y, w, h);
		EndPaint(hwnd, &ps);
	}
	catch (...) {
		MessageBox(NULL, L"failed to preview!", L"Warn", MB_OK); 
	}
}

LRESULT CALLBACK WmaskPreviewWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_PAINT:
		WmaskPreviewOnPaint(hwnd); 
		return 0; 
	case WM_DESTROY:
		PostQuitMessage(0); 
		return 0; 
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam); 
}

void RegisterWmaskPreviewClass() {
	WNDCLASS wc = { 0 }; 
	wc.lpszClassName = L"WmaskPreview"; 
	wc.lpfnWndProc = WmaskPreviewWndProc; 
	wc.hInstance = gHINSTANCE;
	wc.hbrBackground = gBackgroundBrush; 
	RegisterClass(&wc); 
}

HWND CreateWmaskPreviewWindow(HWND parent, int x, int y, int w, int h) {
	return CreateWindowEx(NULL, L"WmaskPreview", NULL, WS_VISIBLE | WS_CHILD, x, y, w, h, parent, NULL, gHINSTANCE, NULL); 
}