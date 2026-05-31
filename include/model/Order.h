#include "BurgerType.h"

#pragma once

class Order {
public:
  BurgerType type;
  int quantity;
  bool isCompleted;
  Order(BurgerType t, int q = 1) : type(t), quantity(q), isCompleted(false) {}
};