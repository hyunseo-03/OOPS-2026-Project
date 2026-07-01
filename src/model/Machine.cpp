#include "model/Machine.h"
#include <cstdlib>

Machine::Machine(const std::string& name, float cycleTime, float malfunctionRate)
    : name(name), state(MachineState::Idle),
      hasCompleted(false), level(1), progress(0.0f),
      cycleTime(cycleTime), malfunctionRate(malfunctionRate),
      baseCycleTime(cycleTime), baseMalfunctionRate(malfunctionRate)
{}

void Machine::checkMalfunction(float dt)
{
    if (state != MachineState::Running) return;
    float roll = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    if (roll < malfunctionRate * dt)
    {
        state = MachineState::Failed;
    }
}

void Machine::setState(MachineState newState)
{
    if (state == MachineState::Failed && newState != MachineState::Idle) return;
    if (newState == MachineState::Paused && state != MachineState::Running) return;

    if (newState == MachineState::Running && state != MachineState::Paused)
    {
        progress = 0.0f;
        hasCompleted = false;
    }

    state = newState;
}

void Machine::finishCycle()
{
    if (state == MachineState::Running) hasCompleted = true;
    state = MachineState::Completed;
}

void Machine::reset()  { state = MachineState::Idle; progress = 0.0f; hasCompleted = false; }

MachineState Machine::getState() const { return state; }
float Machine::getProgress()       const { return progress; }
const std::string& Machine::getName() const { return name; }


void Machine::upgrade()
{
    if (level < maxLevel) {
        level++;
        upgradeCost *= 1.5f;
        cycleTime = baseCycleTime * (1.0f - ((level - 1) * 0.15f));
        malfunctionRate = baseMalfunctionRate * (1.0f - ((level - 1) * 0.20f));
    }
}
