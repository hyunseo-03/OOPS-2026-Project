#pragma once
#include <vector>
#include "enum/BurgerType.h"
#include "struct/FactoryStructs.h"

class OrderManager
{
private:
    Order              currentOrder;
    std::vector<Order> orderQueue;
    std::vector<Order> completedOrders;
    int                completedCount;
    float totalReputation = 0.0f;
public:
    OrderManager();

    void addOrder(BurgerType type);
    const Order& getCurrentOrder() const;
    void completeOrder();
    bool hasOrder() const;
    bool hasActiveOrder() const;

    int getCompletedCount() const;
    const std::vector<Order>& getCompletedOrders() const;
    const std::vector<Order>& getQueuedOrders()    const;
    void updateActiveOrderTime(float dt);
    float getAverageReputation() const;
};
