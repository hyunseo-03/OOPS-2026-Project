#include "model/InventoryManager.h"

bool InventoryManager::hasEnough(IngredientType type, int amount) {
  return inventory[type] >= amount;
}

bool InventoryManager::use(IngredientType type, int amount) {
  if (hasEnough(type, amount)) {
    inventory[type] -= amount;
    return true;
  }
  return false;
}

void InventoryManager::add(IngredientType type, int amount) {
  inventory[type] += amount;
}

int InventoryManager::getAmount(IngredientType type) {
  return inventory[type];
}