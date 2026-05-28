#pragma once

#include "model/BurgerFactoryModel.h"

// ============================================================
// FactoryController - View와 Model을 연결하는 Controller
// MVC에서 Controller 역할:
//   - View에서 버튼 클릭 이벤트를 받아 Model을 변경
//   - 매 프레임 model.update(dt)를 호출
// ============================================================
class FactoryController
{
private:
    BurgerFactoryModel& model;

public:
    FactoryController(BurgerFactoryModel& model);

    // 버튼 이벤트 처리
    void startFactory();
    void stopFactory();
    void newOrder();      // 재료 보충 후 생산 시작
    void sellBurger();    // 완성된 버거 판매

    // 기계별 업그레이드
    bool upgradePrepSpeed();
    bool upgradeGrillSpeed();
    bool upgradeSauceSpeed();
    bool upgradePackSpeed();

    // 기계 일시정지 / 재개
    void togglePauseMachine(int machineIndex); // 0=prep, 1=grill, 2=sauce, 3=pack

    // 매 프레임 호출 (게임 루프에서 호출)
    void update(float dt);
};
