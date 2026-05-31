#pragma once
#include <queue>
#include "BurgerType.h"
#include "Order.h"


class OrderManager{
private: 
  Order currentOrder; 
  std::queue<Order> orderQueue;

public:
  void addOrder(BurgerType type);
  const Order& getCurrentOrder() const;
  void completeOrder();
  bool hasOrder() const;
};