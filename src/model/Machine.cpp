#include "Machine.h"

Machine::Machine(const std::string& name, float cycleTime)
    : name(name), running(false), progress(0.0f), cycleTime(cycleTime) {}

void Machine::start() {
    running = true;
    progress = 0.0f;
}

void Machine::stop() {
    running = false;
}

bool Machine::isDone() const {
    return progress >= 1.0f;
}

float Machine::getProgress() const {
    return progress;
}

const std::string& Machine::getName() const {
    return name;
}