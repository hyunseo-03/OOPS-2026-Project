#pragma once
#include "model/BurgerFactoryModel.h"

// ─────────────────────────────────────────────────────
// FactoryController - View ↔ Model 연결 클래스
//
// View는 버튼 클릭 시 Controller 메서드를 호출한다.
// Controller는 Model의 상태를 변경한다.
// View는 Model을 직접 수정하지 않는다.
// ─────────────────────────────────────────────────────
class FactoryController {
private:
    BurgerFactoryModel& model;

public:
    explicit FactoryController(BurgerFactoryModel& model);

    // 매 프레임 게임 루프에서 호출
    void update(float dt);

    // View 버튼 이벤트 핸들러
    void onStartMachine(ProcessStep step);   // 기계 시작 버튼
    void onNewOrder(BurgerType type);        // 새 주문 버튼
    void onPackBurger();                     // 포장/완료 버튼
    void onRefillInventory();                // 재료 보충 버튼
};
