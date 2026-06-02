#include "model/ProductionLine.h"
#include "model/PrepMachine.h"
#include "model/GrillMachine.h"
#include "model/SauceMachine.h"
#include "model/AssemblyMachine.h"
#include "model/QualityChecker.h"
#include "model/PackingMachine.h"

// 구체 타입 생성은 ProductionLine 생성자에만 존재
// 이후 외부는 Machine* (추상)만 사용
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
        case ProcessStep::GrillPatty:           return 1;
        case ProcessStep::AddSauce:             return 2;
        case ProcessStep::AssembleBurger:       return 3;
        case ProcessStep::QualityCheck:         return 4;
        case ProcessStep::PackBurger:           return 5;
        default:                                return -1;
    }
}

void ProductionLine::update(float dt)
{
    for (auto& m : machines) m->update(dt);
}

// configure() - Machine 추상 인터페이스를 통해 설정값 전달
// 각 기계는 자신에게 필요한 필드만 읽는다
void ProductionLine::configureMachine(ProcessStep step, const MachineConfig& config)
{
    Machine* m = getMachine(step);
    if (m) m->configure(config);
}

void ProductionLine::startMachine(ProcessStep step)
{
    Machine* m = getMachine(step);
    if (m) m->start();
}

bool ProductionLine::isMachineDone(ProcessStep step) const
{
    const Machine* m = getMachine(step);
    return m && m->isDone();
}

bool ProductionLine::isMachineFailed(ProcessStep step) const
{
    const Machine* m = getMachine(step);
    return m && m->isFailed();
}

bool ProductionLine::isMachinePaused(ProcessStep step) const
{
    const Machine* m = getMachine(step);
    return m && m->isPaused();
}

void ProductionLine::pauseMachine(ProcessStep step)
{
    Machine* m = getMachine(step);
    if (m) m->pause();
}

void ProductionLine::resumeMachine(ProcessStep step)
{
    Machine* m = getMachine(step);
    if (m) m->resume();
}

void ProductionLine::repairMachine(ProcessStep step)
{
    Machine* m = getMachine(step);
    if (m) m->repair();
}

Machine* ProductionLine::getMachine(ProcessStep step)
{
    int idx = stepToIndex(step);
    if (idx >= 0 && idx < static_cast<int>(machines.size()))
        return machines[idx].get();
    return nullptr;
}

const Machine* ProductionLine::getMachine(ProcessStep step) const
{
    int idx = stepToIndex(step);
    if (idx >= 0 && idx < static_cast<int>(machines.size()))
        return machines[idx].get();
    return nullptr;
}

void ProductionLine::resetMachine(ProcessStep step)
{
    Machine* m = getMachine(step);
    if (m) m->reset();
}
