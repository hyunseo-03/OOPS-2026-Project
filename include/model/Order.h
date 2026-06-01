#pragma once
#include "BurgerType.h"


class Order {
public:
    BurgerType type;
    int        quantity;
    bool       isCompleted;

    Order()
        : type(BurgerType::CLASSIC), quantity(1), isCompleted(true) {}

    Order(BurgerType t, int q = 1)
        : type(t), quantity(q), isCompleted(false) {}
};
