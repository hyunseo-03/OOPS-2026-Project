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
    void onTogglePause(ProcessStep step);   // 일시정지 / 재개 토글
    bool onUpgradeMachine(ProcessStep step);
};
