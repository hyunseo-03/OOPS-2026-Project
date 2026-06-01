#include "model/InventoryManager.h"

InventoryManager::InventoryManager()
{
    refill();
}

void InventoryManager::refill()
{
    inventory[IngredientType::BUN]    = 20;
    inventory[IngredientType::PATTY]  = 20;
    inventory[IngredientType::LETTUCE]= 20;
    inventory[IngredientType::TOMATO] = 20;
    inventory[IngredientType::ONION]  = 20;
    inventory[IngredientType::SAUCE]  = 20;
    inventory[IngredientType::CHEESE] = 20;
}

bool InventoryManager::hasEnough(IngredientType type, int amount) const
{
    auto it = inventory.find(type);
    if (it == inventory.end()) return false;
    return it->second >= amount;
}

bool InventoryManager::use(IngredientType type, int amount)
{
    if (!hasEnough(type, amount)) return false;
    inventory[type] -= amount;
    return true;
}

void InventoryManager::add(IngredientType type, int amount)
{
    inventory[type] += amount;
}

int InventoryManager::getAmount(IngredientType type) const
{
    auto it = inventory.find(type);
    if (it == inventory.end()) return 0;
    return it->second;
}
