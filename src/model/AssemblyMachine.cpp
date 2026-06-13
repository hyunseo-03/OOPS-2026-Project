#include "model/AssemblyMachine.h"

AssemblyMachine::AssemblyMachine()
    : Machine("Assembly Machine", 4.0f, 0.03f), assembled(false) {}

void AssemblyMachine::update(float dt)
{
    checkMalfunction(dt);
    if (getState() != MachineState::Running) return;

    progress += dt / cycleTime;
    if (progress >= 1.0f)
    {
        progress  = 1.0f;
        assembled = true;
        finishCycle();
    }
}

bool AssemblyMachine::isAssembled() const { return assembled; }
