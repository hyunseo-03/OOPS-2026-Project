#pragma once
#include <map>
#include <vector>
#include "ProcessStep.h"
#include "IngredientType.h"
#include "model/MachineConfig.h"
#include "model/InventoryManager.h"
#include "model/MoneyManager.h"
#include "model/OrderManager.h"
#include "model/ProductionLine.h"

class BurgerFactoryModel
{
private:
    ProcessStep currentStep;
    std::map<IngredientType, int> preparedIngredients;
    bool qualityCheckPassed;

    InventoryManager inventoryManager;
    MoneyManager     moneyManager;
    OrderManager     orderManager;
    ProductionLine   productionLine;

    bool consumeIngredients();
    bool checkQuality() const;
    void handleMalfunction();
    void enterStep(ProcessStep step, bool autoStart = false, const MachineConfig* config = nullptr);

public:
    BurgerFactoryModel();
    static constexpr int REPAIR_COST = 20;

    void update(float dt);
    void startProcess(ProcessStep step);
    bool canProceed(ProcessStep step) const;
    void nextStep();
    void addOrder(BurgerType type);
    void packBurger();
    bool repairMachine(ProcessStep step);
    void togglePauseMachine(ProcessStep step);
    void refillInventory();

    ProcessStep  getCurrentStep()          const;
    int          getTotalBurgersProduced() const;
    int          getMoney()                const;
    bool         hasOrder()                const;
    const Order& getCurrentOrder()         const;
    Machine*     getMachine(ProcessStep step);
    int          getIngredientAmount(IngredientType type) const;
    bool         isQualityCheckPassed()    const;
    bool         isCurrentMachineFailed()  const;
    bool         isCurrentMachinePaused()  const;
    const std::map<IngredientType, int>& getPreparedIngredients() const;
    const std::vector<Order>& getCompletedOrders() const;
    const std::vector<Order>& getQueuedOrders()    const;
};
