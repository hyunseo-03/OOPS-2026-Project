#pragma once
#include <vector>
#include "BurgerType.h"
#include "IngredientType.h"

// ─────────────────────────────────────────────────────
// MachineConfig - 기계 설정값 전달 구조체
//
// BurgerFactoryModel이 레시피를 분석해 이 구조체를 만들고
// ProductionLine을 통해 Machine에 전달한다.
// 각 기계는 자신에게 필요한 필드만 읽는다.
//
// 이 구조체 덕분에 BurgerFactoryModel은 구체적인 기계 타입을
// 전혀 알 필요가 없다 → Loosely Coupled
// ─────────────────────────────────────────────────────
struct MachineConfig
{
    int                          pattyCount    = 1;
    int                          sauceAmount   = 1;
    std::vector<IngredientType>  assemblyOrder = {};
    BurgerType                   burgerType    = BurgerType::CLASSIC;
};
