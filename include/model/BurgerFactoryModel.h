#pragma once

#include "model/Burger.h"
#include "model/Inventory.h"
#include "model/PrepMachine.h"
#include "model/GrillMachine.h"
#include "model/SauceMachine.h"
#include "model/PackingMachine.h"
#include <string>

// ============================================================
// ProcessStep - 현재 공정 단계를 나타내는 enum
// Idle → Preparing → Grilling → Saucing → Packing → Done
// ============================================================
enum class ProcessStep
{
    Idle,
    Preparing,
    Grilling,
    Saucing,
    Packing,
    Done
};

// ============================================================
// BurgerFactoryModel - 공장 전체 데이터 관리 (Model)
// MVC에서 Model 역할: 데이터와 로직만 담당
// ImGui 코드는 절대 들어가지 않음
// ============================================================
class BurgerFactoryModel
{
private:
    Burger       burger;
    Inventory    inventory;

    PrepMachine   prepMachine;
    GrillMachine  grillMachine;
    SauceMachine  sauceMachine;
    PackingMachine packingMachine;

    ProcessStep currentStep;

    int  money;          // 현재 보유 금액
    int  burgerCount;    // 완성된 버거 총 수
    int  readyBurgers;   // 판매 대기 중인 버거 수
    int  orders;         // 주문 수
    int  maxOrders;      // 최대 주문 수 (UI 표시용)
    int  reputation;     // 평판 (1~5)
    bool producing;      // 현재 생산 중인지

public:
    BurgerFactoryModel();

    // 생산 제어
    void startProduction();
    void stopProduction();
    void reset();
    void update(float dt);

    // 액션
    void addIngredients();
    void sellBurger();          // 완성된 버거 판매

    // 업그레이드 (돈을 소비해 기계 성능 향상)
    bool upgradePrepSpeed();
    bool upgradeGrillSpeed();
    bool upgradeSauceSpeed();
    bool upgradePackSpeed();

    // --- Getters ---
    int  getMoney()         const;
    int  getBurgerCount()   const;
    int  getReadyBurgers()  const;
    int  getOrders()        const;
    int  getMaxOrders()     const;
    int  getReputation()    const;
    bool isProducing()      const;

    ProcessStep getCurrentStep() const;
    std::string getStatusMessage() const; // 현재 상태 메시지

    const Burger&        getBurger()        const;
    const Inventory&     getInventory()     const;
    const PrepMachine&   getPrepMachine()   const;
    const GrillMachine&  getGrillMachine()  const;
    const SauceMachine&  getSauceMachine()  const;
    const PackingMachine& getPackingMachine() const;

    // 업그레이드용 non-const 접근자
    PrepMachine&    getPrepMachineRef();
    GrillMachine&   getGrillMachineRef();
    SauceMachine&   getSauceMachineRef();
    PackingMachine& getPackingMachineRef();
};
