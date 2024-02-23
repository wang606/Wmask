#include "WmaskImage.h"

std::vector<std::wstring> globImagePaths(std::wstring dirPath);
Rect _GetImageGeometry(int parentW, int parentH, int imageW, int imageH, const WmaskConfig* pWC);
bool wmaskImageUpdateResource(HWND hwnd);
bool wmaskImageUpdateSize(HWND hwnd); 
bool wmaskImageUpdateImage(HWND hwnd); 

bool wmaskImageOnTimeout(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
bool wmaskImageUpdateSlot(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);
bool wmaskImageOnDestroy(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result); 

LRESULT CALLBACK WmaskImageWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result; 
	if (wmaskImageOnTimeout(hwnd, uMsg, wParam, lParam, result)) return result; 
	if (wmaskImageUpdateSlot(hwnd, uMsg, wParam, lParam, result)) return result;
	if (wmaskImageOnDestroy(hwnd, uMsg, wParam, lParam, result)) return result; 
	return DefWindowProc(hwnd, uMsg, wParam, lParam); 
}

void RegisterWmaskImageClass() {
	WNDCLASS wc = { 0 }; 
	wc.lpszClassName = L"WmaskImage"; 
	wc.lpfnWndProc = WmaskImageWndProc; 
	wc.hInstance = gHINSTANCE; 
	RegisterClass(&wc); 
}

HWND CreateWmaskImageWindow(HWND parent, const WmaskConfig* pWC) {
	WmaskImageData* pData = new WmaskImageData(); 
	pData->wc = pWC; 
	pData->parentHwnd = parent; 
	pData->imagePaths = globImagePaths(pWC->dirpath); 
	if (pData->imagePaths.size() < 1) {
		// MessageBox(NULL, (L"no image found under directory: " + pWC->dirpath).c_str(), L"Error", MB_ICONERROR | MB_OK); 
		delete pData; 
		return NULL; 
	}
	pData->curIdx = rand() % pData->imagePaths.size();

	HWND hwnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT, L"WmaskImage", NULL, WS_VISIBLE, 0, 0, 10, 10, NULL, NULL, gHINSTANCE, NULL);
	SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) | WS_CHILD);
	SetParent(hwnd, parent);
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pData);
	wmaskImageUpdateResource(hwnd); 
	wmaskImageUpdateSize(hwnd); 
	wmaskImageUpdateImage(hwnd); 
	UpdateWindow(hwnd); 
	SetTimer(hwnd, 0, wmaskImageRefreshDuration, NULL); 
	return hwnd; 
}

std::vector<std::wstring> globImagePaths(std::wstring dirPath) {
	std::vector<std::wstring> imagePaths;
	std::wstring imagePath, imageFileName;
	WIN32_FIND_DATA fileData;  
	HANDLE hFind = FindFirstFile((dirPath + L"\\*").c_str(), &fileData);
	if (hFind != INVALID_HANDLE_VALUE) {
		while (FindNextFile(hFind, &fileData)) {
			imagePath = dirPath + L"\\" + fileData.cFileName;
			CharLower(fileData.cFileName); 
			imageFileName = fileData.cFileName; 
			if (imageFileName.ends_with(L".bmp") ||
				imageFileName.ends_with(L".icon") ||
				imageFileName.ends_with(L".jpeg") || 
				imageFileName.ends_with(L".jpg") || 
				imageFileName.ends_with(L".exif") || 
				imageFileName.ends_with(L".png") || 
				imageFileName.ends_with(L".tiff") || 
				imageFileName.ends_with(L".wmf") || 
				imageFileName.ends_with(L".emf")) {
				imagePaths.push_back(imagePath);
			}
		}
	}
	return imagePaths; 
}

Rect _GetImageGeometry(int parentW, int parentH, int imageW, int imageH, const WmaskConfig* pWC) {
	int x = 0, y = 0, w = 0, h = 0; 
	switch (pWC->size) {
	case WmaskConfig::GeometrySize::GS_FIXED:
		w = int((double)imageW * pWC->scale / 100); 
		h = int((double)imageH * pWC->scale / 100); 
		break; 
	case WmaskConfig::GeometrySize::GS_FIT:
		if (imageW * parentH > parentW * imageH) {
			w = int((double)parentW * pWC->scale / 100); 
			h = int((double)w * imageH / imageW); 
		}
		else {
			h = int((double)parentH * pWC->scale / 100); 
			w = int((double)h * imageW / imageH); 
		}
		break; 
	case WmaskConfig::GeometrySize::GS_FILL:
		if (imageW * parentH > parentW * imageH) {
			h = int((double)parentH * pWC->scale / 100);
			w = int((double)h * imageW / imageH);
		}
		else {
			w = int((double)parentW * pWC->scale / 100);
			h = int((double)w * imageH / imageW);
		}
		break; 
	}
	switch (pWC->horizon) {
	case WmaskConfig::GeometryHorizon::GH_CENTER:
		x = (parentW - w) / 2 + pWC->xshift; 
		break;
	case WmaskConfig::GeometryHorizon::GH_LEFT:
		x = 0 + pWC->xshift;
		break;
	case WmaskConfig::GeometryHorizon::GH_RIGHT:
		x = parentW - w + pWC->xshift;
		break;
	}
	switch (pWC->vertical) {
	case WmaskConfig::GeometryVertical::GV_CENTER:
		y = (parentH - h) / 2 + pWC->yshift;
		break;
	case WmaskConfig::GeometryVertical::GV_UP:
		y = 0 + pWC->yshift;
		break;
	case WmaskConfig::GeometryVertical::GV_DOWN:
		y = parentH - h + pWC->yshift;
		break;
	}
	return Rect(x, y, w, h); 
}

bool wmaskImageUpdateResource(HWND hwnd) {
	WmaskImageData* pData = (WmaskImageData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (pData->curImage != NULL) delete pData->curImage; 
	switch (pData->wc.playmode) {
	case WmaskConfig::PlayMode::PM_REPEAT:
		break; 
	case WmaskConfig::PlayMode::PM_LOOP:
		pData->curIdx = (pData->curIdx + 1) % pData->imagePaths.size(); 
		break;
	case WmaskConfig::PlayMode::PM_SHUFFLE:
		pData->curIdx = rand() % pData->imagePaths.size(); 
		break;
	}
	pData->curImage = Image::FromFile(pData->imagePaths[pData->curIdx].c_str()); 
	if (pData->curImage->GetLastStatus() != Status::Ok) {
		MessageBox(NULL, (L"failed to load image: " + pData->imagePaths[pData->curIdx]).c_str(), L"Error", MB_ICONERROR | MB_OK);
		return false;
	}
	return true; 
}

bool wmaskImageUpdateSize(HWND hwnd) {
	WmaskImageData* pData = (WmaskImageData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	RECT parentRect;
	GetClientRect(pData->parentHwnd, &parentRect);
	pData->hwndRect = _GetImageGeometry(parentRect.right, parentRect.bottom, pData->curImage->GetWidth(), pData->curImage->GetHeight(), &pData->wc);
	return SetWindowPos(hwnd, HWND_TOP, 0, 0, pData->hwndRect.Width, pData->hwndRect.Height, SWP_NOMOVE); 
}

bool wmaskImageUpdateImage(HWND hwnd) {
	bool result = false;
	WmaskImageData* pData = (WmaskImageData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	POINT ptDst = { pData->hwndRect.X, pData->hwndRect.Y };
	SIZE size = { pData->hwndRect.Width, pData->hwndRect.Height };
	POINT ptSrc = { 0, 0 };
	BLENDFUNCTION blendFunction;
	blendFunction.BlendOp = AC_SRC_OVER;
	blendFunction.BlendFlags = 0;
	blendFunction.SourceConstantAlpha = pData->wc.opacity;
	blendFunction.AlphaFormat = AC_SRC_ALPHA;
	HDC hdc = GetDC(hwnd);
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP memBitmap = CreateCompatibleBitmap(hdc, pData->hwndRect.Width, pData->hwndRect.Height);
	SelectObject(memDC, memBitmap);
	Graphics graphics(memDC);
	graphics.DrawImage(pData->curImage, 0, 0, pData->hwndRect.Width, pData->hwndRect.Height);
	result = UpdateLayeredWindow(hwnd, GetDC(NULL), &ptDst, &size, memDC, &ptSrc, NULL, &blendFunction, ULW_ALPHA);
	DeleteDC(memDC);
	DeleteObject(memBitmap);
	return result;
}

bool wmaskImageUpdateSlot(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_USER_Update) {
		WmaskImageData* pData = (WmaskImageData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		pData->wc = (WmaskConfig*)wParam;
		result = wmaskImageUpdateResource(hwnd) & wmaskImageUpdateSize(hwnd) & wmaskImageUpdateImage(hwnd);
		return true; 
	}
	else return false; 
}

bool wmaskImageOnTimeout(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_TIMER) {
		bool needUpdateResource = false, needUpdateSize = false, needUpdateImage = false; 
		WmaskImageData* pData = (WmaskImageData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (pData == NULL) return true; 
		// check parent window
		if (!_IsValidTopWindow(pData->parentHwnd)) {
			DestroyWindow(hwnd); 
			return true; 
		}
		pData->lastTime += wmaskImageRefreshDuration; 
		// check for image change. 
		if (pData->wc.playmode != WmaskConfig::PlayMode::PM_REPEAT && pData->lastTime > pData->wc.duration) {
			pData->lastTime = 0;
			needUpdateResource = true;
			needUpdateSize = true;
			needUpdateImage = true;
		}
		// check for size change. 
		WINDOWPLACEMENT wndpl = { 0 }; 
		wndpl.length = sizeof(WINDOWPLACEMENT); 
		GetWindowPlacement(pData->parentHwnd, &wndpl); 
		RECT parentRect; 
		GetClientRect(pData->parentHwnd, &parentRect); 
		if (pData->parentCmdShow != wndpl.showCmd || parentRect.right != pData->parentW || parentRect.bottom != pData->parentH) {
			pData->parentCmdShow = wndpl.showCmd; 
			pData->parentW = parentRect.right; 
			pData->parentH = parentRect.bottom; 
			needUpdateSize = true; 
			needUpdateImage = true; 
		}
		if (needUpdateResource) result &= wmaskImageUpdateResource(hwnd); 
		if (needUpdateSize) result &= wmaskImageUpdateSize(hwnd); 
		if (needUpdateImage) result &= wmaskImageUpdateImage(hwnd); 
		return true; 
	}
	else return false; 
}

bool wmaskImageOnDestroy(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
	if (uMsg == WM_DESTROY) {
		WmaskImageData* pData = (WmaskImageData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (pData != NULL) {
			SendMessage(gWmaskMainHwnd, WM_USER_Close, (WPARAM)&pData->wc, (LPARAM)hwnd);
			if (pData->curImage != NULL) delete pData->curImage;
			delete pData;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
		}
		result = 0;
		return true;
	}
	else return false; 
}
