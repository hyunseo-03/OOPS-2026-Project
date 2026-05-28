#include "../include/model/Machine.h"

// ============================================================
// Machine 생성자
// name: 기계 이름, cycleTime: 1사이클 걸리는 시간(초)
// ============================================================
Machine::Machine(const std::string& name, float cycleTime)
    : name(name),
      running(false),
      paused(false),
      progress(0.0f),
      cycleTime(cycleTime),
      baseCycleTime(cycleTime),
      upgradeLevel(1),
      producedCount(0)
{
}

// ============================================================
// update - 매 프레임 호출
// dt: 이전 프레임으로부터 경과한 시간 (초)
// ============================================================
void Machine::update(float dt)
{
    // 작동 중이 아니거나 일시정지면 업데이트하지 않음
    if (!running || paused)
        return;

    progress += dt;

    // 사이클 완료 체크
    if (progress >= cycleTime)
    {
        progress = cycleTime;
        running  = false;
        producedCount++;
    }
}

void Machine::start()
{
    running = true;
    paused  = false;
}

void Machine::stop()
{
    running = false;
    paused  = false;
}

void Machine::reset()
{
    running       = false;
    paused        = false;
    progress      = 0.0f;
}

void Machine::pause()
{
    if (running)
        paused = true;
}

void Machine::resume()
{
    paused = false;
}

bool Machine::isRunning() const
{
    return running && !paused;
}

bool Machine::isDone() const
{
    return progress >= cycleTime;
}

bool Machine::isPaused() const
{
    return paused;
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

float Machine::getRemainingTime() const
{
    float remaining = cycleTime - progress;
    return (remaining > 0.0f) ? remaining : 0.0f;
}

int Machine::getProducedCount() const
{
    return producedCount;
}

std::string Machine::getName() const
{
    return name;
}

// ============================================================
// applySpeedUpgrade - 사이클 시간을 20% 단축
// 업그레이드 레벨이 올라갈수록 빨라짐
// ============================================================
void Machine::applySpeedUpgrade()
{
    upgradeLevel++;
    cycleTime *= 0.8f; // 20% 감소
}

int Machine::getUpgradeLevel() const
{
    return upgradeLevel;
}

int Machine::getSpeedUpgradeCost() const
{
    // 레벨이 높을수록 비용 증가: 500, 1000, 1500, ...
    return upgradeLevel * 500;
}
