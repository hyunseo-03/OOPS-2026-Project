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
    float       progress;
    float       cycleTime;
    float       malfunctionRate;

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
};
