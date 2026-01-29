# Windows Porting Guide

To port this application to Windows, you will need to replace the macOS-specific APIs (Cocoa, IOKit, CoreGraphics) with the Windows API (Win32).

## Key Components

### 1. Preventing Sleep
On macOS, we use `IOPMAssertionCreateWithName`.
On Windows, use `SetThreadExecutionState`.

```cpp
// Prevent Sleep
SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);

// Allow Sleep (Reset)
SetThreadExecutionState(ES_CONTINUOUS);
```

### 2. Moving the Mouse
On macOS, we use `CGEventCreateMouseEvent`.
On Windows, use `SendInput`.

```cpp
#include <windows.h>

void MoveMouse(int dx, int dy) {
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE; // Relative movement
    input.mi.dx = dx;
    input.mi.dy = dy;
    
    SendInput(1, &input, sizeof(INPUT));
}
```

### 3. User Interface
- **System Tray:** The Windows equivalent of the macOS Menu Bar status item is the "System Tray" or "Notification Area".
- **API:** You can use `Shell_NotifyIcon` to create the icon and a hidden window to handle messages (like right-click context menu).
- **Frameworks:** 
    - **Raw Win32:** Efficient but verbose.
    - **Qt / wxWidgets:** Cross-platform but heavier.
    - **.NET (C#):** `NotifyIcon` class in WinForms/WPF is very easy to use.

## Example C++ Core Logic (Windows)

```cpp
#include <windows.h>
#include <thread>
#include <atomic>

// ... Jiggler class structure ...

void Jiggler::runLoop() {
    // ... setup random ...
    
    while (running) {
        // ... sleep logic ...
        
        // Prevent screensaver/sleep implicitly by input, 
        // but explicit SetThreadExecutionState is better for power management.
        
        MoveMouse(1, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        MoveMouse(-1, 0);
    }
}
```
