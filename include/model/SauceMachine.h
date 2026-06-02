#pragma once
#include "model/Machine.h"

// ─────────────────────────────────────────────────────
// SauceMachine - 소스를 뿌리는 기계
// 소스 유무만 관리한다. (양 추적 없음)
// ─────────────────────────────────────────────────────
class SauceMachine : public Machine
{
private:
    bool sauceApplied;  // 소스가 뿌려졌는지 여부

public:
    SauceMachine();
    void update(float dt) override;

    bool isSauceApplied() const;
};
