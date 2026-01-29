#include "Jiggler.h"
#include <random>

Jiggler::Jiggler() : running(false) {}

Jiggler::~Jiggler() {
    stop();
}

void Jiggler::start() {
    if (running) return;
    running = true;

    // 1. Prevent Sleep
    // ES_DISPLAY_REQUIRED: Forces display to be on
    // ES_SYSTEM_REQUIRED: Forces system to be in working state
    // ES_CONTINUOUS: Informs the system that the state being set should remain in effect until the next call
    SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);

    // 2. Start Jiggle Thread
    workerThread = std::thread(&Jiggler::runLoop, this);
}

void Jiggler::stop() {
    if (!running) return;
    running = false;

    if (workerThread.joinable()) {
        workerThread.join();
    }

    // Restore default sleep behavior
    SetThreadExecutionState(ES_CONTINUOUS);
}

bool Jiggler::isRunning() const {
    return running;
}

void Jiggler::runLoop() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> intervalDist(10000, 60000); // 10s to 60s
    std::uniform_int_distribution<> dirDist(0, 1);

    while (running) {
        // Sleep in chunks for responsiveness
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
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE; // Relative movement
    input.mi.dx = dx;
    input.mi.dy = dy;
    
    SendInput(1, &input, sizeof(INPUT));
}
