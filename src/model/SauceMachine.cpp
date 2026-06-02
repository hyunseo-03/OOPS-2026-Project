#include "model/SauceMachine.h"

SauceMachine::SauceMachine()
    : Machine("Sauce Machine", 1.5f, 0.03f), sauceApplied(false) {}

void SauceMachine::update(float dt)
{
    checkMalfunction(dt);
    if (!running || paused) return;

    progress += dt / cycleTime;
    if (progress >= 1.0f)
    {
        progress     = 1.0f;
        sauceApplied = true;
        stop();
    }
}

bool SauceMachine::isSauceApplied() const { return sauceApplied; }
