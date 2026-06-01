#include "model/ProductionLine.h"
#include "model/PrepMachine.h"
#include "model/GrillMachine.h"
#include "model/SauceMachine.h"
#include "model/AssemblyMachine.h"
#include "model/QualityChecker.h"
#include "model/PackingMachine.h"


// index 0 = PreparingIngredients
// index 1 = GrillPatty
// index 2 = AddSauce
// index 3 = AssembleBurger
// index 4 = QualityCheck
// index 5 = PackBurger
ProductionLine::ProductionLine()
{
    machines.push_back(std::make_unique<PrepMachine>());
    machines.push_back(std::make_unique<GrillMachine>());
    machines.push_back(std::make_unique<SauceMachine>());
    machines.push_back(std::make_unique<AssemblyMachine>());
    machines.push_back(std::make_unique<QualityChecker>());
    machines.push_back(std::make_unique<PackingMachine>());
}

int ProductionLine::stepToIndex(ProcessStep step) const
{
    switch (step)
    {
        case ProcessStep::PreparingIngredients: return 0;
        case ProcessStep::GrillPatty: return 1;
        case ProcessStep::AddSauce: return 2;
        case ProcessStep::AssembleBurger: return 3;
        case ProcessStep::QualityCheck: return 4;
        case ProcessStep::PackBurger: return 5;
        default: return -1;
    }
}

void ProductionLine::update(float dt)
{
    for (auto& machine : machines)
        machine->update(dt);
}

void ProductionLine::startMachine(ProcessStep step)
{
    int idx = stepToIndex(step);
    if (idx >= 0 && idx < static_cast<int>(machines.size()))
        machines[idx]->start();
}

bool ProductionLine::isMachineDone(ProcessStep step) const
{
    int idx = stepToIndex(step);
    if (idx >= 0 && idx < static_cast<int>(machines.size()))
        return machines[idx]->isDone();
    return false;
}

Machine* ProductionLine::getMachine(ProcessStep step)
{
    int idx = stepToIndex(step);
    if (idx >= 0 && idx < static_cast<int>(machines.size()))
        return machines[idx].get();
    return nullptr;
}
