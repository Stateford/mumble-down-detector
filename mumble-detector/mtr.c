#include "mtr.h"

// pressed export text button on MTR
void exportText(HWND window)
{
    HWND text_button = GetDlgItem(window, EXPORT_TEXT);
    SendMessage(window, WM_COMMAND, MAKELONG(EXPORT_TEXT, BN_CLICKED), text_button);
}

// presses export HTML button on MTR
void exportHTML(HWND window)
{
    HWND text_button = GetDlgItem(window, EXPORT_HTML);
    SendMessage(window, WM_COMMAND, MAKELONG(EXPORT_HTML, BN_CLICKED), text_button);
}