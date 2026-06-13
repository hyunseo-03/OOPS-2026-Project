#pragma once
#include "model/IFactoryActions.h"

class FactoryController
{
private:
    IFactoryActions& model;
public:
    explicit FactoryController(IFactoryActions& model);

    void update(float dt);
    void onStartMachine(ProcessStep step);
    void onNewOrder(BurgerType type);
    void onRefillInventory();
    void onRepairMachine(ProcessStep step);
    void onTogglePause(ProcessStep step);
    bool onUpgradeMachine(ProcessStep step);
};
