#pragma once
#include "ProcessStep.h"
#include "model/InventoryManager.h"
#include "model/MoneyManager.h"
#include "model/OrderManager.h"
#include "model/ProductionLine.h"

class BurgerFactoryModel
{
private:
    ProcessStep  currentStep;

    InventoryManager inventoryManager;
    MoneyManager     moneyManager;
    OrderManager     orderManager;
    ProductionLine   productionLine;

    // 레시피 기반 재료 소비 (BurgerRecipe 활용)
    bool consumeIngredients();

public:
    BurgerFactoryModel();

    void update(float dt);
    void startProcess(ProcessStep step);
    bool canProceed(ProcessStep step) const;
    void nextStep();

    void addOrder(BurgerType type);
    void packBurger();

    // ── Getters ──────────────────────────────────────
    ProcessStep  getCurrentStep()          const;
    int          getTotalBurgersProduced() const;   // OrderManager에서 위임
    int          getMoney()                const;
    bool         hasOrder()                const;
    const Order& getCurrentOrder()         const;
    Machine*     getMachine(ProcessStep step);
    int          getIngredientAmount(IngredientType type) const;

    void refillInventory();
};
