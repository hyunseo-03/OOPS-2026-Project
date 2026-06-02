#pragma once
#include "model/Machine.h"

// ─────────────────────────────────────────────────────
// AssemblyMachine - 재료를 조립하는 기계
// 조립 완료 여부(assembled)만 관리한다.
// ─────────────────────────────────────────────────────
class AssemblyMachine : public Machine
{
private:
    bool assembled;  // 조립 완료 여부

public:
    AssemblyMachine();
    void update(float dt) override;

    bool isAssembled() const;
};
