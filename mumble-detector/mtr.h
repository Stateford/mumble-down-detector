#pragma once

#include <Windows.h>

#define EXPORT_TEXT 1006
#define EXPORT_HTML 1007
#define MTR_WINDOW_NAME L"WinMTR v0.92 64 bit by Appnor MSP - www.winmtr.net"

void startProcess(wchar_t*);
HWND getHandle();
void exportText(HWND);
void exportHTML(HWND);