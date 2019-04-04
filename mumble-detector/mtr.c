#include "mtr.h"

// start process and wait until it's ready
void startProcess(wchar_t *path)
{
    STARTUPINFO startupInfo;
    memset(&startupInfo, 0, sizeof(STARTUPINFO));

    PROCESS_INFORMATION procInfo;
    memset(&procInfo, 0, sizeof(PROCESS_INFORMATION));

    wchar_t cwd[80];

    GetCurrentDirectoryW(80, cwd);

    CreateProcessW(
        path,
        L"",
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        cwd,
        &startupInfo,
        &procInfo
        );

    WaitForInputIdle(
        procInfo.hProcess,
        INFINITE
    );
}

HWND getHandle()
{
    return FindWindow(NULL, MTR_WINDOW_NAME);
}

// pressed export text button on MTR
void exportText(HWND window)
{
    HWND text_button = GetDlgItem(window, EXPORT_TEXT);
    SendMessage(window, WM_COMMAND, (WPARAM)MAKELONG(EXPORT_TEXT, BN_CLICKED), (LPARAM)text_button);
}

// presses export HTML button on MTR
void exportHTML(HWND window)
{
    HWND text_button = GetDlgItem(window, EXPORT_HTML);
    SendMessage(window, WM_COMMAND, (WPARAM)MAKELONG(EXPORT_HTML, BN_CLICKED), (LPARAM)text_button);
}