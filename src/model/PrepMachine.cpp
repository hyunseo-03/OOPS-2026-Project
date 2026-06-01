#include "model/PrepMachine.h"

PrepMachine::PrepMachine()
    : Machine("Prep Machine", 2.0f) {}

void PrepMachine::update(float dt)
{
    if (!running) return;

    progress += dt / cycleTime;
    if (progress >= 1.0f)
    {
        progress = 1.0f;
        stop();
    }
}
