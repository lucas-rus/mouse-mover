#include "Jiggler.h"
#include <iostream>
#include <random>
#include <ApplicationServices/ApplicationServices.h>

Jiggler::Jiggler() : running(false), assertionID(kIOPMNullAssertionID) {}

Jiggler::~Jiggler() {
    stop();
}

void Jiggler::start() {
    if (running) return;
    running = true;

    // 1. Prevent Sleep
    CFStringRef reason = CFSTR("Mouse Mover Active");
    IOReturn success = IOPMAssertionCreateWithName(
        kIOPMAssertionTypeNoDisplaySleep,
        kIOPMAssertionLevelOn,
        reason,
        &assertionID
    );

    if (success != kIOReturnSuccess) {
        std::cerr << "Failed to create power assertion" << std::endl;
    }

    // 2. Start Jiggle Thread
    workerThread = std::thread(&Jiggler::runLoop, this);
}

void Jiggler::stop() {
    if (!running) return;
    running = false;

    if (workerThread.joinable()) {
        workerThread.join();
    }

    if (assertionID != kIOPMNullAssertionID) {
        IOPMAssertionRelease(assertionID);
        assertionID = kIOPMNullAssertionID;
    }
}

bool Jiggler::isRunning() const {
    return running;
}

void Jiggler::runLoop() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> intervalDist(10000, 60000); // 10s to 60s
    
    // Direction: 0 or 1
    std::uniform_int_distribution<> dirDist(0, 1);

    while (running) {
        // Sleep in small chunks to allow quick stopping
        int sleepTime = intervalDist(gen);
        for (int i = 0; i < sleepTime / 100; ++i) {
            if (!running) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        if (!running) break;

        // Jiggle
        int move = 2;
        int dx = (dirDist(gen) == 0) ? move : -move;
        
        moveMouse(dx, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        moveMouse(-dx, 0);
    }
}

void Jiggler::moveMouse(int dx, int dy) {
    CGEventRef ourEvent = CGEventCreate(NULL);
    if (!ourEvent) return;
    
    CGPoint currentLoc = CGEventGetLocation(ourEvent);
    CFRelease(ourEvent);
    
    CGPoint newLoc = CGPointMake(currentLoc.x + dx, currentLoc.y + dy);
    
    CGEventRef mouseMove = CGEventCreateMouseEvent(
        NULL, 
        kCGEventMouseMoved, 
        newLoc, 
        kCGMouseButtonLeft
    );
    
    if (mouseMove) {
        CGEventPost(kCGHIDEventTap, mouseMove);
        CFRelease(mouseMove);
    }
}
