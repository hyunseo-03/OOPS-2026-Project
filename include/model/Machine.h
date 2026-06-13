#pragma once
#include <string>
#include "struct/FactoryStructs.h"
#include "enum/MachineState.h"

class Machine
{
protected:
    std::string name;
    MachineState state;
    bool        hasCompleted;
    int         level;
    float       progress;
    float       cycleTime;
    float       malfunctionRate;
    const int maxLevel = 5;
    int upgradeCost = 100;
    float baseCycleTime;
    float baseMalfunctionRate;

    void checkMalfunction(float dt);
    void finishCycle();

public:
    Machine(const std::string& name, float cycleTime, float malfunctionRate = 0.0003f);
    virtual ~Machine() = default;

    virtual void configure(const MachineConfig& config) {}
    virtual void update(float dt) = 0;

    void setState(MachineState newState);
    void reset();

    MachineState getState() const;
    float getProgress()       const;
    const std::string& getName() const;
    int getLevel() const { return level; }
    int getMaxLevel() const { return maxLevel; }
    int getUpgradeCost() const { return upgradeCost; }
    float getCycleTime() const { return cycleTime; }
    float getMalfunctionRate() const { return malfunctionRate; }
    void upgrade();
};
