#include "model/OrderManager.h"

OrderManager::OrderManager()
    : currentOrder(), completedCount(0), totalReputation(0.0f) {}

void OrderManager::updateActiveOrderTime(float dt)
{
    if (!currentOrder.isCompleted) {
        currentOrder.timeTaken += dt;
    }
}

void OrderManager::addOrder(BurgerType type)
{
    Order newOrder(type);
    if (currentOrder.isCompleted)
        currentOrder = newOrder;
    else
        orderQueue.push_back(newOrder);
}

const Order& OrderManager::getCurrentOrder() const { return currentOrder; }

void OrderManager::completeOrder()
{
    float targetTime = 20.0f; 
    float penalty = (currentOrder.timeTaken > targetTime) ? (currentOrder.timeTaken - targetTime) * 0.2f : 0.0f;
    currentOrder.earnedReputation = std::max(1.0f, 5.0f - penalty);

    totalReputation += currentOrder.earnedReputation;
    completedOrders.push_back(currentOrder);
    currentOrder.isCompleted = true;
    completedCount++;

    if (!orderQueue.empty())
    {
        currentOrder = orderQueue.front();
        orderQueue.erase(orderQueue.begin());
    }
}

float OrderManager::getAverageReputation() const
{
    if (completedCount == 0) return 5.0f; // 시작은 5점 만점
    return totalReputation / static_cast<float>(completedCount);
}

bool OrderManager::hasOrder()       const { return !currentOrder.isCompleted || !orderQueue.empty(); }
bool OrderManager::hasActiveOrder() const { return !currentOrder.isCompleted; }
int  OrderManager::getCompletedCount()              const { return completedCount; }
const std::vector<Order>& OrderManager::getCompletedOrders() const { return completedOrders; }
const std::vector<Order>& OrderManager::getQueuedOrders()    const { return orderQueue; }
