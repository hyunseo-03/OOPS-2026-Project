#include "model/SauceMachine.h"

SauceMachine::SauceMachine()
    : Machine("Sauce Machine", 1.0f, 0.03f), targetAmount(1), dispensed(0.0f) {}

void SauceMachine::configure(const MachineConfig& config)
{
    targetAmount = config.sauceAmount;
    dispensed    = 0.0f;
    cycleTime    = targetAmount * 1.0f;
}

void SauceMachine::update(float dt)
{
    checkMalfunction(dt);
    if (!running || paused) return;

    dispensed += targetAmount * (dt / cycleTime);
    if (dispensed > targetAmount) dispensed = static_cast<float>(targetAmount);

    progress += dt / cycleTime;
    if (progress >= 1.0f)
    {
        progress  = 1.0f;
        dispensed = static_cast<float>(targetAmount);
        stop();
    }
}

int SauceMachine::getTargetAmount() const { return targetAmount; }
int SauceMachine::getDispensed()    const { return static_cast<int>(dispensed); }
