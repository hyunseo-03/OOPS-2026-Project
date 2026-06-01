#include "model/SauceMachine.h"

SauceMachine::SauceMachine()
    : Machine("Sauce Machine", 1.0f) {}

void SauceMachine::update(float dt)
{
    if (!running) return;

    progress += dt / cycleTime;
    if (progress >= 1.0f)
    {
        progress = 1.0f;
        stop();
    }
}
