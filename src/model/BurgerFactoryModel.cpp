#include "model/BurgerFactoryModel.h"
#include "model/BurgerRecipe.h"

BurgerFactoryModel::BurgerFactoryModel()
    : currentStep(ProcessStep::Idle)
{}

// ─────────────────────────────────────────────────────
// update() - 기계가 완료되면 자동으로 nextStep() 호출
// ─────────────────────────────────────────────────────
void BurgerFactoryModel::update(float dt)
{
    if (currentStep == ProcessStep::Idle ||
        currentStep == ProcessStep::Done)
        return;

    productionLine.update(dt);

    if (productionLine.isMachineDone(currentStep))
        nextStep();
}

// ─────────────────────────────────────────────────────
// startProcess() - Controller의 onStartMachine()이 호출
// PreparingIngredients 단계에서만 재료를 소비한다.
// ─────────────────────────────────────────────────────
void BurgerFactoryModel::startProcess(ProcessStep step)
{
    if (step != currentStep) return;

    if (step == ProcessStep::PreparingIngredients)
    {
        if (!consumeIngredients()) return;  // 재료 부족
    }

    productionLine.startMachine(step);
}

// ─────────────────────────────────────────────────────
// canProceed() - BurgerRecipe를 참조해 재고 확인
// 중복 조건 없이 레시피 한 곳에서 관리한다.
// ─────────────────────────────────────────────────────
bool BurgerFactoryModel::canProceed(ProcessStep step) const
{
    if (!orderManager.hasActiveOrder()) return false;

    if (step == ProcessStep::PreparingIngredients)
    {
        BurgerRecipe recipe = getRecipe(orderManager.getCurrentOrder().type);
        for (const auto& [ingredient, amount] : recipe.ingredients)
        {
            if (!inventoryManager.hasEnough(ingredient, amount))
                return false;
        }
    }

    return true;
}

// ─────────────────────────────────────────────────────
// nextStep() - 단계 전진, GrillPatty 이후는 자동 시작
// ─────────────────────────────────────────────────────
void BurgerFactoryModel::nextStep()
{
    switch (currentStep)
    {
        case ProcessStep::Idle:
            currentStep = ProcessStep::PreparingIngredients;
            break;
        case ProcessStep::PreparingIngredients:
            currentStep = ProcessStep::GrillPatty;
            productionLine.startMachine(currentStep);
            break;
        case ProcessStep::GrillPatty:
            currentStep = ProcessStep::AddSauce;
            productionLine.startMachine(currentStep);
            break;
        case ProcessStep::AddSauce:
            currentStep = ProcessStep::AssembleBurger;
            productionLine.startMachine(currentStep);
            break;
        case ProcessStep::AssembleBurger:
            currentStep = ProcessStep::QualityCheck;
            productionLine.startMachine(currentStep);
            break;
        case ProcessStep::QualityCheck:
            currentStep = ProcessStep::PackBurger;
            productionLine.startMachine(currentStep);
            break;
        case ProcessStep::PackBurger:
            currentStep = ProcessStep::Done;
            break;
        case ProcessStep::Done:
            currentStep = ProcessStep::Idle;
            break;
    }
}

void BurgerFactoryModel::addOrder(BurgerType type)
{
    orderManager.addOrder(type);
    if (currentStep == ProcessStep::Idle)
        currentStep = ProcessStep::PreparingIngredients;
}

// ─────────────────────────────────────────────────────
// packBurger() - Done 상태에서 완료 처리
// 버거 타입별 가격은 BurgerRecipe에서 읽는다.
// ─────────────────────────────────────────────────────
void BurgerFactoryModel::packBurger()
{
    if (currentStep != ProcessStep::Done) return;

    BurgerRecipe recipe = getRecipe(orderManager.getCurrentOrder().type);
    moneyManager.add(recipe.price);
    orderManager.completeOrder();
    currentStep = ProcessStep::Idle;

    if (orderManager.hasActiveOrder())
        currentStep = ProcessStep::PreparingIngredients;
}

// ── Private ──────────────────────────────────────────
// consumeIngredients() - BurgerRecipe 기반 재료 소비
// canProceed()와 같은 레시피를 사용하므로 중복 없음
bool BurgerFactoryModel::consumeIngredients()
{
    BurgerRecipe recipe = getRecipe(orderManager.getCurrentOrder().type);
    for (const auto& [ingredient, amount] : recipe.ingredients)
    {
        if (!inventoryManager.use(ingredient, amount))
            return false;
    }
    return true;
}

// ── Getters ──────────────────────────────────────────
ProcessStep BurgerFactoryModel::getCurrentStep() const
{
    return currentStep;
}

// totalBurgersProduced는 더 이상 멤버 변수가 아님
// OrderManager의 completedCount로 위임
int BurgerFactoryModel::getTotalBurgersProduced() const
{
    return orderManager.getCompletedCount();
}

int BurgerFactoryModel::getMoney() const
{
    return moneyManager.getMoney();
}

bool BurgerFactoryModel::hasOrder() const
{
    return orderManager.hasOrder();
}

const Order& BurgerFactoryModel::getCurrentOrder() const
{
    return orderManager.getCurrentOrder();
}

Machine* BurgerFactoryModel::getMachine(ProcessStep step)
{
    return productionLine.getMachine(step);
}

int BurgerFactoryModel::getIngredientAmount(IngredientType type) const
{
    return inventoryManager.getAmount(type);
}

void BurgerFactoryModel::refillInventory()
{
    inventoryManager.refill();
}
