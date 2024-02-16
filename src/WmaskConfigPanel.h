#pragma once

#include "required.h"

void RegisterWmaskConfigPanelClass(); 
HWND CreateWmaskConfigPanelWindow(HWND parent, const WmaskConfig& wc); 
