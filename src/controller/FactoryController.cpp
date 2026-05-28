#include "controller/FactoryController.h"

FactoryController::FactoryController(BurgerFactoryModel& model)
    : model(model)
{
}

// ============================================================
// startFactory - 생산 시작 버튼 처리
// ============================================================
void FactoryController::startFactory()
{
    model.startProduction();
}

// ============================================================
// stopFactory - 생산 중지 버튼 처리
// ============================================================
void FactoryController::stopFactory()
{
    model.stopProduction();
}

// ============================================================
// newOrder - 새 주문 버튼 처리
// 재료를 보충하고 바로 생산을 시작
// ============================================================
void FactoryController::newOrder()
{
    model.addIngredients();
    model.startProduction();
}

// ============================================================
// sellBurger - 버거 판매 버튼 처리
// ============================================================
void FactoryController::sellBurger()
{
    model.sellBurger();
}

// ============================================================
// 업그레이드 메서드들
// Model에 위임하고 결과(성공/실패)를 반환
// ============================================================
bool FactoryController::upgradePrepSpeed()   { return model.upgradePrepSpeed(); }
bool FactoryController::upgradeGrillSpeed()  { return model.upgradeGrillSpeed(); }
bool FactoryController::upgradeSauceSpeed()  { return model.upgradeSauceSpeed(); }
bool FactoryController::upgradePackSpeed()   { return model.upgradePackSpeed(); }

// ============================================================
// togglePauseMachine - 특정 기계 일시정지 / 재개
// machineIndex: 0=준비, 1=그릴, 2=소스, 3=포장
// ============================================================
void FactoryController::togglePauseMachine(int machineIndex)
{
    Machine* machines[4] = {
        &model.getPrepMachineRef(),
        &model.getGrillMachineRef(),
        &model.getSauceMachineRef(),
        &model.getPackingMachineRef()
    };

    if (machineIndex < 0 || machineIndex > 3)
        return;

    Machine* m = machines[machineIndex];
    if (m->isPaused())
        m->resume();
    else
        m->pause();
}

// ============================================================
// update - 매 프레임 Model을 업데이트
// main 게임 루프에서 호출됨
// ============================================================
void FactoryController::update(float dt)
{
    model.update(dt);
}
