#include "model/AssemblyMachine.h"

AssemblyMachine::AssemblyMachine()
    : Machine("Assembly Machine", 4.0f, 0.03f), stepTimer(0.0f) {}

void AssemblyMachine::configure(const MachineConfig& config)
{
    toAssemble = config.assemblyOrder;
    assembled.clear();
    stepTimer = 0.0f;
    cycleTime = STEP_TIME * static_cast<float>(toAssemble.size());
}

void AssemblyMachine::update(float dt)
{
    checkMalfunction(dt);
    if (!running || paused) return;

    stepTimer += dt;
    if (stepTimer >= STEP_TIME && assembled.size() < toAssemble.size())
    {
        assembled.push_back(toAssemble[assembled.size()]);
        stepTimer = 0.0f;
    }

    progress += dt / cycleTime;
    if (progress >= 1.0f)
    {
        assembled = toAssemble;
        progress  = 1.0f;
        stop();
    }
}

const std::vector<IngredientType>& AssemblyMachine::getAssembled()  const { return assembled; }
const std::vector<IngredientType>& AssemblyMachine::getToAssemble() const { return toAssemble; }
