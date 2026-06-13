#pragma once
#include <vector>
#include "enum/BurgerType.h"
#include "struct/FactoryStructs.h"

class OrderManager
{
private:
    Order              currentOrder;
    std::vector<Order> orderQueue;      // 대기 주문 (vector로 순회 가능)
    std::vector<Order> completedOrders; // 완료 히스토리
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
    void updateActiveOrderTime(float dt); // 추가
    float getAverageReputation() const;  // 대기 주문 목록
};
