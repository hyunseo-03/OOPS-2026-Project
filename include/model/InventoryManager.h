#pragma once
#include <map>
#include "enum/IngredientType.h"

class InventoryManager {
private:
    std::map<IngredientType, int> inventory;

public:
    InventoryManager();

    bool hasEnough(IngredientType type, int amount) const;
    bool use(IngredientType type, int amount);
    void add(IngredientType type, int amount);
    int  getAmount(IngredientType type) const;

    void refill();
};
