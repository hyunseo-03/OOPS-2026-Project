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
    float roll = static_cast<float>(rand()) / RAND_MAX;
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
    if (state == MachineState::Running) hasCompleted = true;  // 실제로 돌다가 멈춰야만 완주로 인정
    state = MachineState::Idle;
}

void Machine::reset()  { state = MachineState::Idle; progress = 0.0f; hasCompleted = false; }

bool  Machine::isDone()    const { return state != MachineState::Failed && hasCompleted && progress >= 1.0f; }
bool  Machine::isFailed()  const { return state == MachineState::Failed; }
bool  Machine::isRunning() const { return state == MachineState::Running; }
bool  Machine::isPaused()  const { return state == MachineState::Paused; }
float Machine::getProgress()       const { return progress; }
const std::string& Machine::getName() const { return name; }


void Machine::upgrade()
{
    if (level < maxLevel) {
        level++;
        upgradeCost *= 1.5f; // 다음 레벨업 비용 1.5배 증가
        // 레벨당 작업 시간은 15%씩 단축하고, 고장 확률은 20%씩 감소시킨다.
        cycleTime = baseCycleTime * (1.0f - ((level - 1) * 0.15f));
        malfunctionRate = baseMalfunctionRate * (1.0f - ((level - 1) * 0.20f));
    }
}
