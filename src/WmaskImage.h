#pragma once

#include "required.h"

const int wmaskImageRefreshDuration = 50;  // ms

struct WmaskImageData {
	WmaskConfig wc; 
	int lastTime; 
	std::vector<std::wstring> imagePaths; 
	int curIdx; 
	Image *curImage;
	HWND parentHwnd; 
	int parentCmdShow, parentW, parentH;
	Rect hwndRect; 
	WmaskImageData() : 
		wc(), 
		lastTime(0), 
		imagePaths(), 
		curIdx(0), 
		curImage(NULL), 
		parentHwnd(NULL), 
		parentCmdShow(SW_NORMAL), 
		parentW(0),
		parentH(0), 
		hwndRect()
	{}
};

void RegisterWmaskImageClass(); 
HWND CreateWmaskImageWindow(HWND parent, const WmaskConfig* pWC); 
