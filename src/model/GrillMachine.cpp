#include "model/GrillMachine.h"

GrillMachine::GrillMachine()
    : Machine("Grill Machine", BASE_CYCLE, 0.04f),
      pattyCount(1), pattiesGrilled(0) {}

void GrillMachine::configure(const MachineConfig& config)
{
    pattyCount     = config.pattyCount;
    pattiesGrilled = 0;
}

void GrillMachine::update(float dt)
{
    checkMalfunction(dt);
    if (getState() != MachineState::Running) return;

    progress += dt / cycleTime;

    if (progress >= 1.0f)
    {
        pattiesGrilled++;

        if (pattiesGrilled < pattyCount)
        {
            progress = 0.0f;
        }
        else
        {
            progress = 1.0f;
            finishCycle();
        }
    }
}

int GrillMachine::getPattyCount()     const { return pattyCount; }
int GrillMachine::getPattiesGrilled() const { return pattiesGrilled; }
