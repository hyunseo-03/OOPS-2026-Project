#include "model/GrillMachine.h"

GrillMachine::GrillMachine()
    : Machine("Grill Machine", 3.0f) {}

void GrillMachine::update(float dt)
{
    if (!running) return;

    progress += dt / cycleTime;
    if (progress >= 1.0f)
    {
        progress = 1.0f;
        stop();
    }
}
