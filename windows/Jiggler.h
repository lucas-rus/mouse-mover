#pragma once

#include <windows.h>
#include <thread>
#include <atomic>

class Jiggler {
public:
    Jiggler();
    ~Jiggler();

    void start();
    void stop();
    bool isRunning() const;

private:
    void runLoop();
    void moveMouse(int dx, int dy);

    std::atomic<bool> running;
    std::thread workerThread;
};
