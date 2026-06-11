#pragma once
#include "BurgerType.h"
#include "ProcessStep.h"

class IFactoryActions
{
public:
    virtual ~IFactoryActions() = default;

    virtual void update(float dt) = 0;
    virtual void startProcess(ProcessStep step) = 0;
    virtual void addOrder(BurgerType type) = 0;
    virtual void refillInventory() = 0;
    virtual bool repairMachine(ProcessStep step) = 0;
    virtual void togglePauseMachine(ProcessStep step) = 0;
    virtual bool upgradeMachine(ProcessStep step) = 0;
};
