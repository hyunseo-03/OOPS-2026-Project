#pragma once
#include <string>
#include "model/MachineConfig.h"

class Machine
{
protected:
    std::string name;
    bool        running;
    bool        paused;
    bool        failed;
    bool        hasCompleted;   // start() 후 stop()까지 완주했는지 여부
    int         level;           // 기계 레벨 (업그레이드 시 증가)
    float       progress;
    float       cycleTime;
    float       malfunctionRate;
    int level = 1;               // 현재 레벨
    const int maxLevel = 5;      // 최대 레벨
    int upgradeCost = 100;       // 업그레이드 비용 (기본값)
    float baseCycleTime;

    void checkMalfunction(float dt);

public:
    Machine(const std::string& name, float cycleTime, float malfunctionRate = 0.0003f);
    virtual ~Machine() = default;

    virtual void configure(const MachineConfig& config) {}
    virtual void update(float dt) = 0;

    void start();
    void stop();
    void reset();
    void repair();
    void pause();
    void resume();

    bool  isDone()    const;
    bool  isFailed()  const;
    bool  isRunning() const;
    bool  isPaused()  const;
    float getProgress()       const;
    const std::string& getName() const;
    int getLevel() const { return level; }
    int getMaxLevel() const { return maxLevel; }
    int getUpgradeCost() const { return upgradeCost; }
    
    // 업그레이드 실행 함수
    void upgrade();
};
