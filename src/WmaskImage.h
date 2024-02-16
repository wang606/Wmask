#pragma once

#include "required.h"

const int wmaskImageRefreshDuration = 50;  // ms

struct WmaskImageData {
	WmaskConfig wc; 
	bool fading; 
	int lastTime; 
	std::vector<std::wstring> imagePaths; 
	int curIdx, nextIdx; 
	Image *curImage, *nextImage;
	HWND parentHwnd; 
	int parentCmdShow, parentW, parentH;
	Rect hwndRect, curRect, nextRect; 
	WmaskImageData() : 
		wc(), 
		fading(false), 
		lastTime(0), 
		imagePaths(), 
		curIdx(0), 
		nextIdx(0), 
		curImage(NULL), 
		nextImage(NULL), 
		parentHwnd(NULL), 
		parentCmdShow(SW_NORMAL), 
		parentW(0),
		parentH(0), 
		hwndRect(), 
		curRect(), 
		nextRect()
	{}
};

void RegisterWmaskImageClass(); 
HWND CreateWmaskImageWindow(HWND parent, const WmaskConfig* pWC); 
