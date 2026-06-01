#include "controller/FactoryController.h"

FactoryController::FactoryController(BurgerFactoryModel& model)
    : model(model) {}

// ─────────────────────────────────────────────────────
// update() - 매 프레임 Model을 업데이트
// main 게임 루프(ImGui render loop)에서 호출됨
// ─────────────────────────────────────────────────────
void FactoryController::update(float dt)
{
    model.update(dt);
}

// ─────────────────────────────────────────────────────
// onStartMachine() - 기계 시작 버튼 처리
// 조건 확인 후 model에 작업 시작 요청
// ─────────────────────────────────────────────────────
void FactoryController::onStartMachine(ProcessStep step)
{
    if (!model.canProceed(step)) return;
    model.startProcess(step);
}

// ─────────────────────────────────────────────────────
// onNewOrder() - 새 주문 버튼 처리
// ─────────────────────────────────────────────────────
void FactoryController::onNewOrder(BurgerType type)
{
    model.addOrder(type);
}

// ─────────────────────────────────────────────────────
// onPackBurger() - 포장 완료 버튼 처리
// Done 상태일 때 버거를 완성 처리하고 돈을 받는다.
// ─────────────────────────────────────────────────────
void FactoryController::onPackBurger()
{
    model.packBurger();
}

// ─────────────────────────────────────────────────────
// onRefillInventory() - 재료 보충 버튼 처리
// ─────────────────────────────────────────────────────
void FactoryController::onRefillInventory()
{
    model.refillInventory();
}
