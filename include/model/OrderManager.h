#pragma once
#include <queue>
#include <vector>
#include "BurgerType.h"
#include "model/Order.h"

class OrderManager
{
private:
    Order             currentOrder;
    std::queue<Order> orderQueue;
    int               completedCount;
    std::vector<Order> completedOrders;  // 완료된 주문 히스토리

public:
    OrderManager();

    void addOrder(BurgerType type);
    const Order& getCurrentOrder() const;
    void completeOrder();
    bool hasOrder() const;
    bool hasActiveOrder() const;

    int getCompletedCount() const;
    const std::vector<Order>& getCompletedOrders() const;
};
