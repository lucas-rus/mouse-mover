# Mouse Mover for macOS

A simple, lightweight menu bar application that prevents your computer from going to sleep by periodically simulating minute mouse movements.

## Features

- **Prevent Sleep:** Uses macOS Power Management assertions (`IOPMAssertion`) to prevent display sleep.
- **Subtle Movement:** Jiggles the mouse cursor slightly (invisible to most users) every 10-60 seconds to keep the session active.
- **Menu Bar Icon:** Unobtrusive status bar icon. Green when active, default color when inactive.
- **Lightweight:** Written in C++ and Objective-C++ for minimal resource usage.

## Compatibility

- **macOS:** Requires macOS 10.10 (Yosemite) or later.
- **Architecture:** Compatible with both Intel (x86_64) and Apple Silicon (arm64) Macs.

## Building

You can build this using `clang` directly or within Xcode.

```bash
clang++ -framework Cocoa -framework IOKit -framework ApplicationServices -std=c++11 -o MouseMover Jiggler.mm main.mm
```

## License

MIT License
