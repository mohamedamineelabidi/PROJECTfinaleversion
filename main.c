#define WINVER 0x0500
#define _WIN32_IE 0x0500

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "medicine.h"

#define MAX_MEDICINES 100

int main(void) {
    // Initialize COM for modern Windows controls
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);

    // Create and show the main window
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOWDEFAULT);
}