#pragma once
#include <string>
#include <vector>
#include "enum/IngredientType.h"
#include "model/Order.h"
#include "enum/ProcessStep.h"
#include "enum/MachineState.h"

class IFactoryViewData
{
public:
    virtual ~IFactoryViewData() = default;

    virtual ProcessStep getCurrentStep() const = 0;
    virtual int getTotalBurgersProduced() const = 0;
    virtual int getMoney() const = 0;
    virtual bool hasOrder() const = 0;
    virtual const Order& getCurrentOrder() const = 0;
    virtual int getIngredientAmount(IngredientType type) const = 0;
    virtual MachineState getCurrentMachineState() const = 0;
    virtual bool isGameOver() const = 0;
    virtual const std::string& getStatusMessage() const = 0;
    virtual const std::vector<Order>& getCompletedOrders() const = 0;
    virtual const std::vector<Order>& getQueuedOrders() const = 0;
    virtual float getAverageReputation() const = 0;
    virtual int getRepairCost() const = 0;

    virtual bool hasMachine(ProcessStep step) const = 0;
    virtual MachineState getMachineState(ProcessStep step) const = 0;
    virtual float getMachineProgress(ProcessStep step) const = 0;
    virtual const std::string& getMachineName(ProcessStep step) const = 0;
    virtual int getMachineLevel(ProcessStep step) const = 0;
    virtual int getMachineMaxLevel(ProcessStep step) const = 0;
    virtual int getMachineUpgradeCost(ProcessStep step) const = 0;
    virtual float getMachineCycleTime(ProcessStep step) const = 0;
    virtual float getMachineMalfunctionRate(ProcessStep step) const = 0;
};
