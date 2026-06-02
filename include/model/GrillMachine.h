#pragma once
#include "model/Machine.h"

// ─────────────────────────────────────────────────────
// GrillMachine - 패티를 굽는 기계
// pattyCount만큼 사이클을 반복한다.
// Double 버거 → 패티 2개 → 사이클 2회
// ─────────────────────────────────────────────────────
class GrillMachine : public Machine
{
private:
    int pattyCount;      // 총 구워야 할 패티 수
    int pattiesGrilled;  // 완료된 패티 수

    static constexpr float BASE_CYCLE = 3.0f;  // 패티 1개당 굽는 시간(초)

public:
    GrillMachine();
    void configure(const MachineConfig& config) override;
    void update(float dt) override;

    int getPattyCount()     const;
    int getPattiesGrilled() const;
};
