#pragma once
#include <vector>
#include <memory>
#include "model/Machine.h"
#include "ProcessStep.h"

// ─────────────────────────────────────────────────────
// ProductionLine - 모든 기계를 보유 (Composition)
//
// [Loosely Coupled]
// 외부(BurgerFactoryModel)는 Machine 추상 타입만 사용한다.
// GrillMachine, SauceMachine 등 구체 타입은 이 클래스 내부에만 존재.
// configureMachine()으로 MachineConfig를 전달해 설정한다.
// ─────────────────────────────────────────────────────
class ProductionLine
{
private:
    std::vector<std::unique_ptr<Machine>> machines;
    int stepToIndex(ProcessStep step) const;

public:
    ProductionLine();

    void update(float dt);
    void configureMachine(ProcessStep step, const MachineConfig& config);
    void startMachine(ProcessStep step);
    bool isMachineDone(ProcessStep step)   const;
    bool isMachineFailed(ProcessStep step) const;
    bool isMachinePaused(ProcessStep step) const;

    void pauseMachine(ProcessStep step);
    void resumeMachine(ProcessStep step);
    void repairMachine(ProcessStep step);

    Machine*       getMachine(ProcessStep step);
    const Machine* getMachine(ProcessStep step) const;
};
