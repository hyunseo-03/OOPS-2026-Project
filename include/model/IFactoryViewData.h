#pragma once
#include <string>
#include <vector>
#include "IngredientType.h"
#include "model/Order.h"
#include "ProcessStep.h"

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
    virtual bool isCurrentMachineFailed() const = 0;
    virtual bool isCurrentMachinePaused() const = 0;
    virtual bool isGameOver() const = 0;
    virtual const std::string& getStatusMessage() const = 0;
    virtual const std::vector<Order>& getCompletedOrders() const = 0;
    virtual const std::vector<Order>& getQueuedOrders() const = 0;
    virtual float getAverageReputation() const = 0;
    virtual int getRepairCost() const = 0;

    virtual bool hasMachine(ProcessStep step) const = 0;
    virtual bool isMachineRunning(ProcessStep step) const = 0;
    virtual bool isMachinePaused(ProcessStep step) const = 0;
    virtual bool isMachineFailed(ProcessStep step) const = 0;
    virtual bool isMachineDone(ProcessStep step) const = 0;
    virtual float getMachineProgress(ProcessStep step) const = 0;
    virtual const std::string& getMachineName(ProcessStep step) const = 0;
    virtual int getMachineLevel(ProcessStep step) const = 0;
    virtual int getMachineMaxLevel(ProcessStep step) const = 0;
    virtual int getMachineUpgradeCost(ProcessStep step) const = 0;
    virtual float getMachineCycleTime(ProcessStep step) const = 0;
    virtual float getMachineMalfunctionRate(ProcessStep step) const = 0;
};
