#pragma once

#include "required.h"

const int wmaskMainRefreshDuration = 100;  // ms

void RegisterWmaskMainClass(); 
HWND CreateWmaskMainWindow();

void OpenConfig();
void SaveConfig();