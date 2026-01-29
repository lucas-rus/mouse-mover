#pragma once

#include <thread>
#include <atomic>
#include <IOKit/pwr_mgt/IOPMLib.h>

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
    IOPMAssertionID assertionID;
};
