#pragma once

#include <Windows.h>

#define EXPORT_TEXT 1006
#define EXPORT_HTML 1007
#define MTR_WINDOW_NAME L"WinMTR v0.92 64 bit by Appnor MSP - www.winmtr.net"

typedef struct _SAVEASCONTROLS_
{
    HWND edit;
    HWND save;
} SAVEASCONTROLS;

PROCESS_INFORMATION startProcess(wchar_t*);
void closeProcess(PROCESS_INFORMATION*);
HWND getHandle();
void exportText(HWND);
void exportHTML(HWND);

void enumSaveAsChildren(HWND, SAVEASCONTROLS*);
BOOL CALLBACK EnumWindowsProc(HWND, LPARAM);

void saveFile(wchar_t*, SAVEASCONTROLS);