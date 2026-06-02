#pragma once
#include "model/BurgerFactoryModel.h"

class FactoryController
{
private:
    BurgerFactoryModel& model;
public:
    explicit FactoryController(BurgerFactoryModel& model);

    void update(float dt);
    void onStartMachine(ProcessStep step);
    void onNewOrder(BurgerType type);
    void onPackBurger();
    void onRefillInventory();
    void onRepairMachine(ProcessStep step);
    void onTogglePause(ProcessStep step);   // 일시정지 / 재개 토글
};
