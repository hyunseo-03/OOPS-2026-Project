#include "../include/model/Machine.h"

//cycleTime 기계가 작업을 완료하는데 걸리는 시간
//progress 현재 작업 진행 상황 (0.0f ~ cycleTime)
//running 기계가 현재 작동 중인지 여부
Machine::Machine(const std::string& name, float cycleTime)
    : name(name),
      running(false),
      progress(0.0f),
      cycleTime(cycleTime)
{
}

void Machine::update(float dt)
{
    if (!running)
        return;

    progress += dt;

    if (progress >= cycleTime)
    {
        progress = cycleTime;
        running = false;
    }
}

void Machine::start()
{
    running = true;
}

void Machine::stop()
{
    running = false;
}

void Machine::reset()
{
    running = false;
    progress = 0.0f;
}

bool Machine::isRunning() const
{
    return running;
}

bool Machine::isDone() const
{
    return progress >= cycleTime;
}

float Machine::getProgress() const
{
    if (cycleTime <= 0.0f)
        return 0.0f;

    return progress / cycleTime;
}

float Machine::getCycleTime() const
{
    return cycleTime;
}

std::string Machine::getName() const
{
    return name;
}