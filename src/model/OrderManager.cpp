#include "model/OrderManager.h"
#include "model/Order.h"

void OrderManager::addOrder(BurgerType type) {
  orderQueue.push(Order(type));
}

const Order& OrderManager::getCurrentOrder() const {
  return currentOrder;
}

void OrderManager::completeOrder() {
  currentOrder.isCompleted = true;
}

bool OrderManager::hasOrder() const {
  return !orderQueue.empty() || !currentOrder.isCompleted;
}