# Mouse Mover

A simple, lightweight application that prevents your computer from going to sleep or being flagged as afk by periodically simulating minute mouse movements.

## Features

- **Prevent Sleep:** Keeps your system awake.
- **Counters Datadog/Work Afk Detection Systems:** Prevents from being flagged as afk.
- **Subtle Movement:** Jiggles the mouse cursor slightly every 10-60 seconds.
- **System Tray / Menu Bar:** Unobtrusive icon with Start/Stop controls.
- **Lightweight:** Minimal resource usage.

## macOS

### Requirements
- macOS 10.10 (Yosemite) or later.
- Compatible with Intel and Apple Silicon.

### Building (macOS)
```bash
clang++ -framework Cocoa -framework IOKit -framework ApplicationServices -std=c++11 -o MouseMover src/Jiggler.mm src/main.mm
```

## Windows

### Usage
Simply run `MouseMover.exe` (included in the repository).

### Requirements
- Windows 7, 8, 10, or 11.

### Building (Windows)
If you want to build from source, you can use Visual Studio (MSVC) or MinGW.

**Using MSVC (Visual Studio Developer Command Prompt):**
```cmd
cl /EHsc /DUNICODE /D_UNICODE windows/main.cpp windows/Jiggler.cpp /link user32.lib kernel32.lib shell32.lib
```

**Using MinGW (g++):**
```bash
g++ -DUNICODE -D_UNICODE -municode windows/main.cpp windows/Jiggler.cpp -o MouseMover.exe -static -mwindows
```

## License

MIT License
