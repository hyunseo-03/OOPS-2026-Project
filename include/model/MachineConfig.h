#pragma once
#include "enum/BurgerType.h"

// ─────────────────────────────────────────────────────
// MachineConfig - 기계 설정값 전달 구조체
// ─────────────────────────────────────────────────────
struct MachineConfig
{
    int        pattyCount = 1;                    // GrillMachine용
    BurgerType burgerType = BurgerType::CLASSIC;  // PackingMachine용
};
