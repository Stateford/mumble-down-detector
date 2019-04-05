#include "mtr.h"
#include <string.h>

// start process and wait until it's ready
PROCESS_INFORMATION startProcess(wchar_t *path)
{
    STARTUPINFO startupInfo;
    memset(&startupInfo, 0, sizeof(STARTUPINFO));

    PROCESS_INFORMATION procInfo;
    memset(&procInfo, 0, sizeof(PROCESS_INFORMATION));

    wchar_t cwd[200];

    const DWORD buflen = GetCurrentDirectoryW(200, cwd);

    CreateProcess(
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
    
    return procInfo;
}

void closeProcess(PROCESS_INFORMATION* pInfo)
{
    TerminateProcess(pInfo->hProcess, 0);
    CloseHandle(pInfo->hProcess, 0);
    CloseHandle(pInfo->hThread, 0);
}

HWND getHandle()
{
    return FindWindow(NULL, MTR_WINDOW_NAME);
}

// pressed export text button on MTR
void exportText(HWND window)
{
    HWND text_button = GetDlgItem(window, EXPORT_TEXT);
    PostMessage(window, WM_COMMAND, (WPARAM)MAKELONG(EXPORT_TEXT, BN_CLICKED), (LPARAM)text_button);
}

// presses export HTML button on MTR
void exportHTML(HWND window)
{
    HWND text_button = GetDlgItem(window, EXPORT_HTML);
    PostMessage(window, WM_COMMAND, (WPARAM)MAKELONG(EXPORT_HTML, BN_CLICKED), (LPARAM)text_button);
}

void enumSaveAsChildren(HWND parent, SAVEASCONTROLS *controls)
{
    EnumChildWindows(parent, &EnumWindowsProc, (LPARAM)controls);
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    static BOOL edit_found = FALSE;
    static BOOL save_found = FALSE;

    wchar_t className[500];
    GetClassName(hwnd, className, 500);
    const HINSTANCE instance = (HINSTANCE)GetModuleHandle(L"Save As");
    WNDCLASS classInfo;
    memset(&classInfo, 0, sizeof(WNDCLASS));

    GetClassInfo(instance, className, &classInfo);
    WINDOWINFO wInfo;
    memset(&wInfo, 0, sizeof(WINDOWINFO));
    GetWindowInfo(hwnd, &wInfo);


    if (lstrcmp(className, L"Edit") == 0 && !edit_found)
    {
        SAVEASCONTROLS* controls = (SAVEASCONTROLS*)lParam;
        controls->edit = hwnd;
        edit_found = TRUE;
    }

    if (lstrcmp(className, L"Button") == 0 && !save_found)
    {
        wchar_t *caption[200];
        GetWindowText(hwnd, caption, 200);
        
        if (lstrcmp(caption, L"&Save") == 0)
        {
            SAVEASCONTROLS* controls = (SAVEASCONTROLS*)lParam;
            controls->save = hwnd;
            save_found = TRUE;
        }
    }

    return TRUE;
}

void saveFile(wchar_t* path, SAVEASCONTROLS controls)
{
    SendMessage(controls.edit, WM_SETTEXT, 0, path);
    SendMessage(controls.save, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
    SendMessage(controls.save, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(0, 0));
}