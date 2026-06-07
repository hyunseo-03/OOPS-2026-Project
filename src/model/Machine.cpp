#include "model/Machine.h"
#include <cstdlib>

Machine::Machine(const std::string& name, float cycleTime, float malfunctionRate)
    : name(name), running(false), paused(false), failed(false),
      hasCompleted(false), level(1), progress(0.0f),
      cycleTime(cycleTime), malfunctionRate(malfunctionRate),
      baseCycleTime(cycleTime)
{}

void Machine::checkMalfunction(float dt)
{
    if (!running || paused || failed) return;
    float roll = static_cast<float>(rand()) / RAND_MAX;
    if (roll < malfunctionRate * dt)
    {
        running = false;
        failed  = true;
    }
}

void Machine::start()
{
    if (failed) return;
    running      = true;
    paused       = false;
    progress     = 0.0f;
    hasCompleted = false;   // 새 사이클 시작 → 완주 플래그 초기화
}

void Machine::stop()
{
    if (running) hasCompleted = true;  // 실제로 돌다가 멈춰야만 완주로 인정
    running = false;
}

void Machine::reset()  { running = false; paused = false; progress = 0.0f; failed = false; hasCompleted = false; }
void Machine::repair() { failed = false; running = false; paused = false; progress = 0.0f; hasCompleted = false; }
void Machine::pause()  { if (running) paused = true; }
void Machine::resume() { if (paused) paused = false; }

bool  Machine::isDone()    const { return !failed && hasCompleted && progress >= 1.0f; }
bool  Machine::isFailed()  const { return failed; }
bool  Machine::isRunning() const { return running && !paused; }
bool  Machine::isPaused()  const { return paused; }
float Machine::getProgress()       const { return progress; }
const std::string& Machine::getName() const { return name; }


void Machine::upgrade()
{
    if (level < maxLevel) {
        level++;
        upgradeCost *= 1.5f; // 다음 레벨업 비용 1.5배 증가
        // 사이클 타임 감소 (예: 레벨당 15% 단축)
        cycleTime = baseCycleTime * (1.0f - ((level - 1) * 0.15f)); 
    }
}
