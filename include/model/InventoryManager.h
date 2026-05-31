#pragma once

#include <map>
#include "IngredientType.h"

class InventoryManager {
private:
  std::map<IngredientType, int> inventory;
public:
  bool hasEnough(IngredientType type, int amount);
  bool use(IngredientType type, int amount);
  void add(IngredientType type, int amount);
  int getAmount(IngredientType type);
};