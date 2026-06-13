#pragma once

#include <map>

#include "enum/BurgerType.h"
#include "enum/IngredientType.h"

struct Order {
    BurgerType type = BurgerType::CLASSIC;
    bool       isCompleted = false;
    float      timeTaken = 0.0f;
    float      earnedReputation = 5.0f;

    Order() {}

    Order(BurgerType t)
        : type(t), isCompleted(false), timeTaken(0.0f), earnedReputation(5.0f) {}
};

struct BurgerRecipe
{
    std::map<IngredientType, int> ingredients;
    int price;
};

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

struct MachineConfig
{
    int        pattyCount = 1;
    BurgerType burgerType = BurgerType::CLASSIC;
};
