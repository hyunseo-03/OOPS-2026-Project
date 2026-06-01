#include "model/OrderManager.h"

OrderManager::OrderManager()
    : currentOrder(), completedCount(0) {}

void OrderManager::addOrder(BurgerType type)
{
    Order newOrder(type);

    if (currentOrder.isCompleted)
        currentOrder = newOrder;
    else
        orderQueue.push(newOrder);
}

const Order& OrderManager::getCurrentOrder() const
{
    return currentOrder;
}

void OrderManager::completeOrder()
{
    currentOrder.isCompleted = true;
    completedCount++;

    if (!orderQueue.empty())
    {
        currentOrder = orderQueue.front();
        orderQueue.pop();
    }
}

bool OrderManager::hasOrder() const
{
    return !currentOrder.isCompleted || !orderQueue.empty();
}

bool OrderManager::hasActiveOrder() const
{
    return !currentOrder.isCompleted;
}

int OrderManager::getCompletedCount() const
{
    return completedCount;
}
