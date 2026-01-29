# Mouse Mover

A simple, lightweight application that prevents your computer from going to sleep by periodically simulating minute mouse movements.

## Features

- **Prevent Sleep:** Keeps your system awake.
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

### Requirements
- Windows 7, 8, 10, or 11.

### Building (Windows)
You can build this using Visual Studio (MSVC) or MinGW.

**Using MSVC (Visual Studio Developer Command Prompt):**
```cmd
cl /EHsc windows/main.cpp windows/Jiggler.cpp /link user32.lib kernel32.lib shell32.lib
```

**Using MinGW (g++):**
```bash
g++ windows/main.cpp windows/Jiggler.cpp -o MouseMover.exe -static -mwindows
```

## License

MIT License