#pragma once
#include "enum/BurgerType.h"

struct Order {
    BurgerType type = BurgerType::CLASSIC;
    int        quantity = 1;
    bool       isCompleted = false;
    float      timeTaken = 0.0f;          // 주문 처리하는 데 걸린 누적 시간
    float      earnedReputation = 5.0f;   // 이 버거로 얻은 평판
    
    // 기본 생성자
    Order() {}
    
    // 버거 종류와 수량을 지정하는 생성자 (수량은 기본값 1)
    Order(BurgerType t, int q = 1) 
        : type(t), quantity(q), isCompleted(false), timeTaken(0.0f), earnedReputation(5.0f) {}
};
