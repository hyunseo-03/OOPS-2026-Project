#include "model/PrepMachine.h"

PrepMachine::PrepMachine() : Machine("PrepMachine", 3.0f) {}

void PrepMachine::update(float dt)
{
    checkMalfunction(dt);
    if (getState() != MachineState::Running) return;

    progress += dt / cycleTime;
    if (progress >= 1.0f) { progress = 1.0f; finishCycle(); }
}
