#pragma once
#include <string>
#include "model/MachineConfig.h"

// ─────────────────────────────────────────────────────
// Machine - 추상 기계 클래스 (Abstract Base Class)
//
// [Loosely Coupled 설계]
// 외부(ProductionLine, BurgerFactoryModel)는 이 추상 클래스만 안다.
// 구체 기계 타입(GrillMachine 등)에 직접 의존하지 않는다.
//
// configure() : 작업 시작 전 설정값 주입 (기본은 빈 구현)
//               각 서브클래스가 필요한 필드만 override해서 읽는다.
// update()    : pure virtual - 반드시 서브클래스에서 구현
// ─────────────────────────────────────────────────────
class Machine
{
protected:
    std::string name;
    bool        running;
    bool        paused;         // 일시정지 상태
    float       progress;
    float       cycleTime;
    bool        failed;
    float       malfunctionRate;

    void checkMalfunction(float dt);

public:
    Machine(const std::string& name, float cycleTime, float malfunctionRate = 0.005f);
    virtual ~Machine() = default;

    // 설정값 주입 - 서브클래스가 필요한 필드만 override
    virtual void configure(const MachineConfig& config) {}

    virtual void update(float dt) = 0;

    void start();
    void stop();
    void reset();
    void repair();
    void pause();
    void resume();

    bool  isDone()       const;
    bool  isFailed()     const;
    bool  isRunning()    const;
    bool  isPaused()     const;
    float getProgress()  const;
    const std::string& getName() const;
};
