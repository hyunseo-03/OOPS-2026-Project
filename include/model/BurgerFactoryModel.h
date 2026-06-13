#pragma once
#include <map>
#include <string>
#include <vector>
#include "enum/ProcessStep.h"
#include "enum/IngredientType.h"
#include "struct/FactoryStructs.h"
#include "model/IFactoryActions.h"
#include "model/IFactoryViewData.h"
#include "model/InventoryManager.h"
#include "model/MoneyManager.h"
#include "model/OrderManager.h"
#include "model/ProductionLine.h"

class BurgerFactoryModel : public IFactoryActions, public IFactoryViewData
{
private:
    ProcessStep currentStep;
    std::map<IngredientType, int> preparedIngredients;
    bool qualityCheckPassed;
    bool productionTimerRunning;
    bool gameOver;
    std::string statusMessage;

    InventoryManager inventoryManager;
    MoneyManager     moneyManager;
    OrderManager     orderManager;
    ProductionLine   productionLine;

    bool consumeIngredients();
    bool checkQuality() const;
    void handleMalfunction();
    void enterStep(ProcessStep step, bool autoStart = false, const MachineConfig* config = nullptr);
    bool canProceed(ProcessStep step) const;
    void nextStep();

public:
    BurgerFactoryModel();
    static constexpr int REPAIR_COST = 20;
    static constexpr int REFILL_COST = 100;

private:
    // Controller command interface
    void update(float dt) override;
    void startProcess(ProcessStep step) override;
    void addOrder(BurgerType type) override;
    bool repairMachine(ProcessStep step) override;
    void togglePauseMachine(ProcessStep step) override;
    void refillInventory() override;
    bool upgradeMachine(ProcessStep step) override;

    // View read-only interface
    ProcessStep  getCurrentStep()          const override;
    int          getTotalBurgersProduced() const override;
    int          getMoney()                const override;
    bool         hasOrder()                const override;
    const Order& getCurrentOrder()         const override;
    int          getIngredientAmount(IngredientType type) const override;
    MachineState getCurrentMachineState() const override;
    bool         isGameOver()              const override;
    const std::string& getStatusMessage()  const override;
    const std::vector<Order>& getCompletedOrders() const override;
    const std::vector<Order>& getQueuedOrders()    const override;
    float        getAverageReputation() const override;
    int          getRepairCost() const override;

    bool hasMachine(ProcessStep step) const override;
    MachineState getMachineState(ProcessStep step) const override;
    float getMachineProgress(ProcessStep step) const override;
    const std::string& getMachineName(ProcessStep step) const override;
    int getMachineLevel(ProcessStep step) const override;
    int getMachineMaxLevel(ProcessStep step) const override;
    int getMachineUpgradeCost(ProcessStep step) const override;
    float getMachineCycleTime(ProcessStep step) const override;
    float getMachineMalfunctionRate(ProcessStep step) const override;
};
