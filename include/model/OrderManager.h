#pragma once
#include <queue>
#include "BurgerType.h"
#include "model/Order.h"

// ─────────────────────────────────────────────────────
// OrderManager - 주문 관리 클래스
// 현재 주문 + 대기 큐 + 완료 카운트를 관리한다.
// totalBurgersProduced는 여기서 관리한다.
// ─────────────────────────────────────────────────────
class OrderManager
{
private:
    Order             currentOrder;
    std::queue<Order> orderQueue;
    int               completedCount;   // 완성된 버거 총 개수

public:
    OrderManager();

    void addOrder(BurgerType type);
    const Order& getCurrentOrder() const;
    void completeOrder();
    bool hasOrder() const;
    bool hasActiveOrder() const;

    int getCompletedCount() const;
};
