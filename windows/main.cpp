#include <windows.h>
#include <shellapi.h>
#include <string>
#include "Jiggler.h"

// ID for the taskbar icon
#define WM_TRAYICON (WM_USER + 1)
#define ID_TRAY_APP_ICON 1001

// Menu Command IDs
#define ID_TRAY_EXIT  2001
#define ID_TRAY_TOGGLE 2002

// Global variables
NOTIFYICONDATA nid;
HWND hWnd;
HMENU hMenu;
Jiggler g_jiggler;

// Forward declarations
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void InitTrayIcon(HWND hwnd);
void RemoveTrayIcon();
void ShowContextMenu(HWND hwnd);
void UpdateMenuState();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    // 1. Register Window Class (Hidden window to handle messages)
    const wchar_t CLASS_NAME[] = L"MouseMoverHiddenWindow";
    
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    
    RegisterClass(&wc);
    
    // 2. Create Hidden Window
    hWnd = CreateWindowEx(
        0, CLASS_NAME, L"Mouse Mover Host",
        0, 0, 0, 0, 0,
        HWND_MESSAGE, NULL, hInstance, NULL
    );
    
    if (hWnd == NULL) return 0;
    
    // 3. Initialize Tray Icon
    InitTrayIcon(hWnd);
    
    // 4. Start Jiggler immediately (optional, mirroring mac behavior of 'ready' but not started? 
    // Mac version starts with "Start" button available. Let's keep it stopped initially.)
    
    // 5. Message Loop
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    RemoveTrayIcon();
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_TRAYICON:
        if (lParam == WM_RBUTTONUP || lParam == WM_LBUTTONUP) {
            ShowContextMenu(hwnd);
        }
        break;
        
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_TRAY_TOGGLE:
            if (g_jiggler.isRunning()) {
                g_jiggler.stop();
            } else {
                g_jiggler.start();
            }
            break;
            
        case ID_TRAY_EXIT:
            PostQuitMessage(0);
            break;
        }
        break;
        
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
        
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void InitTrayIcon(HWND hwnd) {
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = ID_TRAY_APP_ICON;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Default application icon
    lstrcpyW(nid.szTip, L"Mouse Mover");
    
    Shell_NotifyIcon(NIM_ADD, &nid);
}

void RemoveTrayIcon() {
    Shell_NotifyIcon(NIM_DELETE, &nid);
}

void ShowContextMenu(HWND hwnd) {
    POINT pt;
    GetCursorPos(&pt);
    
    HMENU hMenu = CreatePopupMenu();
    
    if (hMenu) {
        if (g_jiggler.isRunning()) {
             AppendMenu(hMenu, MF_STRING, ID_TRAY_TOGGLE, L"Stop");
             // Optional: Add a checkmark or change icon color logic here if we had custom icons
        } else {
             AppendMenu(hMenu, MF_STRING, ID_TRAY_TOGGLE, L"Start");
        }
        
        AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
        AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT, L"Quit Mouse Mover");
        
        // This is required to make the menu disappear if the user clicks outside
        SetForegroundWindow(hwnd); 
        TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, NULL);
        DestroyMenu(hMenu);
    }
}
