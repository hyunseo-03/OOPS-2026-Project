#pragma once
#include <string>
#include "model/MachineConfig.h"
#include "enum/MachineState.h"

class Machine
{
protected:
    std::string name;
    MachineState state;
    bool        hasCompleted;   // 현재 작업 사이클을 완주했는지 여부
    int         level;           // 기계 레벨 (업그레이드 시 증가)
    float       progress;
    float       cycleTime;
    float       malfunctionRate;
    const int maxLevel = 5;      // 최대 레벨
    int upgradeCost = 100;       // 업그레이드 비용 (기본값)
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
    
    // 업그레이드 실행 함수
    void upgrade();
};
