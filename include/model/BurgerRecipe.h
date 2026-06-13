#pragma once
#include <map>
#include "enum/IngredientType.h"
#include "enum/BurgerType.h"

// ─────────────────────────────────────────────────────
// BurgerRecipe - 버거 타입별 필요 재료 정의
// canProceed()와 consumeIngredients() 둘 다 여기서 참조한다.
// 레시피 변경이 필요하면 이 파일만 수정하면 된다.
// ─────────────────────────────────────────────────────
struct BurgerRecipe
{
    std::map<IngredientType, int> ingredients;  // 재료 → 필요 수량
    int price;                                  // 판매 가격
};

// 버거 타입에 맞는 레시피 반환
inline BurgerRecipe getRecipe(BurgerType type)
{
    switch (type)
    {
        case BurgerType::CLASSIC:
            return {
                {
                    { IngredientType::BUN,     1 },
                    { IngredientType::PATTY,   1 },
                    { IngredientType::CHEESE,  1 },
                    { IngredientType::LETTUCE, 1 },
                    { IngredientType::TOMATO,  1 },
                    { IngredientType::SAUCE,   1 },
                },
                100
            };

        case BurgerType::CHEESE:
            return {
                {
                    { IngredientType::BUN,     1 },
                    { IngredientType::PATTY,   1 },
                    { IngredientType::CHEESE,  2 },
                    { IngredientType::LETTUCE, 1 },
                    { IngredientType::SAUCE,   1 },
                },
                130
            };

        case BurgerType::DOUBLE:
            return {
                {
                    { IngredientType::BUN,     1 },
                    { IngredientType::PATTY,   2 },
                    { IngredientType::CHEESE,  1 },
                    { IngredientType::LETTUCE, 1 },
                    { IngredientType::TOMATO,  1 },
                    { IngredientType::SAUCE,   1 },
                },
                160
            };

        default:
            return {};
    }
}
