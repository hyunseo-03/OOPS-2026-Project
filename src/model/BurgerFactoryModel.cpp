#include "model/BurgerFactoryModel.h"

BurgerFactoryModel::BurgerFactoryModel()
    : currentStep(ProcessStep::Idle),
      money(1250),
      burgerCount(0),
      readyBurgers(0),
      orders(0),
      maxOrders(10),
      reputation(4),
      producing(false)
{
}

// ============================================================
// startProduction - 버거 생산 시작
// 재료가 충분하고 현재 생산 중이 아닐 때만 시작 가능
// ============================================================
void BurgerFactoryModel::startProduction()
{
    if (producing)
        return;

    if (!inventory.hasIngredients())
        return;

    burger.reset();
    inventory.useIngredients();

    producing   = true;
    currentStep = ProcessStep::Preparing;

    prepMachine.reset();
    prepMachine.start();
}

void BurgerFactoryModel::stopProduction()
{
    producing = false;

    prepMachine.stop();
    grillMachine.stop();
    sauceMachine.stop();
    packingMachine.stop();

    currentStep = ProcessStep::Idle;
}

void BurgerFactoryModel::reset()
{
    burger.reset();

    prepMachine.reset();
    grillMachine.reset();
    sauceMachine.reset();
    packingMachine.reset();

    currentStep   = ProcessStep::Idle;
    producing     = false;
    burgerCount   = 0;
    readyBurgers  = 0;
    orders        = 0;
    money         = 1250;
    reputation    = 4;
}

// ============================================================
// update - 매 프레임 호출되는 핵심 로직
// 현재 단계에 맞는 기계를 업데이트하고 단계를 전환함
// ============================================================
void BurgerFactoryModel::update(float dt)
{
    if (!producing)
        return;

    switch (currentStep)
    {
    case ProcessStep::Preparing:
        prepMachine.update(dt);
        if (prepMachine.isDone())
        {
            burger.setPrepared(true);
            currentStep = ProcessStep::Grilling;
            grillMachine.reset();
            grillMachine.start();
        }
        break;

    case ProcessStep::Grilling:
        grillMachine.update(dt);
        if (grillMachine.isDone())
        {
            burger.setGrilled(true);
            currentStep = ProcessStep::Saucing;
            sauceMachine.reset();
            sauceMachine.start();
        }
        break;

    case ProcessStep::Saucing:
        sauceMachine.update(dt);
        if (sauceMachine.isDone())
        {
            burger.setSauced(true);
            currentStep = ProcessStep::Packing;
            packingMachine.reset();
            packingMachine.start();
        }
        break;

    case ProcessStep::Packing:
        packingMachine.update(dt);
        if (packingMachine.isDone())
        {
            burger.setPacked(true);
            burgerCount++;
            readyBurgers++;
            orders++;

            // 주문 달성마다 평판 상승
            if (orders % 5 == 0 && reputation < 5)
                reputation++;

            currentStep = ProcessStep::Done;
            producing   = false;
        }
        break;

    case ProcessStep::Idle:
    case ProcessStep::Done:
    default:
        break;
    }
}

void BurgerFactoryModel::addIngredients()
{
    inventory.addIngredients();
}

// ============================================================
// sellBurger - 대기 중인 버거를 판매하고 돈을 획득
// ============================================================
void BurgerFactoryModel::sellBurger()
{
    if (readyBurgers <= 0)
        return;

    // 평판이 높을수록 버거당 가격 증가
    int price = 100 + (reputation - 1) * 20;
    money += price;
    readyBurgers--;
}

// ============================================================
// 업그레이드 메서드들
// 돈이 충분하면 해당 기계의 속도를 업그레이드
// ============================================================
bool BurgerFactoryModel::upgradePrepSpeed()
{
    int cost = prepMachine.getSpeedUpgradeCost();
    if (money < cost) return false;
    money -= cost;
    prepMachine.applySpeedUpgrade();
    return true;
}

bool BurgerFactoryModel::upgradeGrillSpeed()
{
    int cost = grillMachine.getSpeedUpgradeCost();
    if (money < cost) return false;
    money -= cost;
    grillMachine.applySpeedUpgrade();
    return true;
}

bool BurgerFactoryModel::upgradeSauceSpeed()
{
    int cost = sauceMachine.getSpeedUpgradeCost();
    if (money < cost) return false;
    money -= cost;
    sauceMachine.applySpeedUpgrade();
    return true;
}

bool BurgerFactoryModel::upgradePackSpeed()
{
    int cost = packingMachine.getSpeedUpgradeCost();
    if (money < cost) return false;
    money -= cost;
    packingMachine.applySpeedUpgrade();
    return true;
}

// ============================================================
// getStatusMessage - 현재 공정 단계를 사람이 읽기 좋은 문자열로 반환
// ============================================================
std::string BurgerFactoryModel::getStatusMessage() const
{
    switch (currentStep)
    {
    case ProcessStep::Idle:      return "대기 중...";
    case ProcessStep::Preparing: return "재료 준비 중...";
    case ProcessStep::Grilling:  return "패티 굽는 중...";
    case ProcessStep::Saucing:   return "소스 추가 중...";
    case ProcessStep::Packing:   return "포장 중...";
    case ProcessStep::Done:      return "버거 완성! 판매하세요.";
    default:                     return "";
    }
}

// --- Getters ---
int  BurgerFactoryModel::getMoney()        const { return money; }
int  BurgerFactoryModel::getBurgerCount()  const { return burgerCount; }
int  BurgerFactoryModel::getReadyBurgers() const { return readyBurgers; }
int  BurgerFactoryModel::getOrders()       const { return orders; }
int  BurgerFactoryModel::getMaxOrders()    const { return maxOrders; }
int  BurgerFactoryModel::getReputation()   const { return reputation; }
bool BurgerFactoryModel::isProducing()     const { return producing; }

ProcessStep BurgerFactoryModel::getCurrentStep() const { return currentStep; }

const Burger&         BurgerFactoryModel::getBurger()         const { return burger; }
const Inventory&      BurgerFactoryModel::getInventory()      const { return inventory; }
const PrepMachine&    BurgerFactoryModel::getPrepMachine()    const { return prepMachine; }
const GrillMachine&   BurgerFactoryModel::getGrillMachine()   const { return grillMachine; }
const SauceMachine&   BurgerFactoryModel::getSauceMachine()   const { return sauceMachine; }
const PackingMachine& BurgerFactoryModel::getPackingMachine() const { return packingMachine; }

PrepMachine&    BurgerFactoryModel::getPrepMachineRef()    { return prepMachine; }
GrillMachine&   BurgerFactoryModel::getGrillMachineRef()   { return grillMachine; }
SauceMachine&   BurgerFactoryModel::getSauceMachineRef()   { return sauceMachine; }
PackingMachine& BurgerFactoryModel::getPackingMachineRef() { return packingMachine; }
