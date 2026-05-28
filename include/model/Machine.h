#pragma once

#include <string>

// ============================================================
// Machine - 추상 기계 베이스 클래스
// 모든 공장 기계의 공통 기능을 담당합니다.
// GrillMachine, PrepMachine 등이 이 클래스를 상속합니다.
// ============================================================

class Machine
{
protected:
    std::string name;          // 기계 이름
    bool        running;       // 현재 작동 중인지 여부
    bool        paused;        // 일시정지 여부
    float       progress;      // 현재 진행 시간 (초 단위, 0 ~ cycleTime)
    float       cycleTime;     // 1사이클 완료까지 걸리는 시간 (초)
    float       baseCycleTime; // 업그레이드 전 원래 사이클 시간
    int         upgradeLevel;  // 업그레이드 레벨 (기본값 1)
    int         producedCount; // 완료된 사이클 횟수

public:
    Machine(const std::string& name, float cycleTime);
    virtual ~Machine() = default;

    // 매 프레임 호출 - 자식 클래스에서 override 가능
    virtual void update(float dt);

    // 기계 제어
    void start();
    void stop();
    void reset();
    void pause();
    void resume();

    // 상태 확인
    bool isRunning()  const;
    bool isDone()     const;
    bool isPaused()   const;

    // 진행 정보
    float getProgress()      const; // 0.0 ~ 1.0 비율
    float getCycleTime()     const;
    float getRemainingTime() const; // 남은 시간 (초)
    int   getProducedCount() const;

    // 기계 정보
    std::string getName() const;

    // 업그레이드
    void applySpeedUpgrade();        // 사이클 시간 20% 단축
    int  getUpgradeLevel()      const;
    int  getSpeedUpgradeCost()  const; // 레벨에 따른 업그레이드 비용
};
