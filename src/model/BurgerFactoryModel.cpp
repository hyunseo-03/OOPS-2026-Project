#include "model/BurgerFactoryModel.h"
#include "model/BurgerRecipe.h"

BurgerFactoryModel::BurgerFactoryModel()
    : currentStep(ProcessStep::Idle), qualityCheckPassed(false) {}

void BurgerFactoryModel::update(float dt)
{
    if (currentStep == ProcessStep::Idle || currentStep == ProcessStep::Done) return;

    productionLine.update(dt);
    handleMalfunction();

    if (productionLine.isMachineDone(currentStep))
        nextStep();
}

void BurgerFactoryModel::handleMalfunction()
{
    // 고장 감지: 단계는 그대로 유지 (수리 후 재시작 가능하도록)
    // update()에서 isMachineDone이 true가 되지 않으므로 자동으로 멈춘다
}

bool BurgerFactoryModel::repairMachine(ProcessStep step)
{
    if (!productionLine.isMachineFailed(step)) return false;
    if (!moneyManager.spend(REPAIR_COST))      return false;
    productionLine.repairMachine(step);
    // 수리 완료 후 해당 단계 기계 자동 재시작
    if (step == currentStep)
        productionLine.startMachine(step);
    return true;
}

void BurgerFactoryModel::togglePauseMachine(ProcessStep step)
{
    if (productionLine.isMachinePaused(step))
        productionLine.resumeMachine(step);
    else
        productionLine.pauseMachine(step);
}

void BurgerFactoryModel::startProcess(ProcessStep step)
{
    if (step != currentStep) return;
    if (productionLine.isMachineFailed(step)) return;

    if (step == ProcessStep::PreparingIngredients)
        if (!consumeIngredients()) return;

    productionLine.startMachine(step);
}

bool BurgerFactoryModel::canProceed(ProcessStep step) const
{
    if (!orderManager.hasActiveOrder()) return false;
    if (productionLine.isMachineFailed(step)) return false;

    if (step == ProcessStep::PreparingIngredients)
    {
        BurgerRecipe recipe = getRecipe(orderManager.getCurrentOrder().type);
        for (const auto& [ingredient, amount] : recipe.ingredients)
            if (!inventoryManager.hasEnough(ingredient, amount)) return false;
    }
    return true;
}

// ─────────────────────────────────────────────────────
// nextStep() - 다음 단계로 이동
// configureMachine()으로 MachineConfig를 넘긴다.
// BurgerFactoryModel은 구체 기계 타입을 전혀 모른다.
// ─────────────────────────────────────────────────────
void BurgerFactoryModel::nextStep()
{
    BurgerType   type   = orderManager.getCurrentOrder().type;
    BurgerRecipe recipe = getRecipe(type);
    MachineConfig config;
    config.burgerType = type;

    switch (currentStep)
    {
        case ProcessStep::Idle:
            currentStep = ProcessStep::PreparingIngredients;
            break;

        case ProcessStep::PreparingIngredients:
            config.pattyCount = recipe.ingredients.count(IngredientType::PATTY)
                              ? recipe.ingredients.at(IngredientType::PATTY) : 1;
            currentStep = ProcessStep::GrillPatty;
            productionLine.configureMachine(currentStep, config);
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
            qualityCheckPassed = checkQuality();
            if (qualityCheckPassed)
            {
                currentStep = ProcessStep::PackBurger;
                productionLine.configureMachine(currentStep, config);
                productionLine.startMachine(currentStep);
            }
            else
            {
                preparedIngredients.clear();
                orderManager.completeOrder();
                currentStep = ProcessStep::Idle;
                if (orderManager.hasActiveOrder())
                    currentStep = ProcessStep::PreparingIngredients;
            }
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

void BurgerFactoryModel::packBurger()
{
    if (currentStep != ProcessStep::Done) return;
    BurgerRecipe recipe = getRecipe(orderManager.getCurrentOrder().type);
    moneyManager.add(recipe.price);
    orderManager.completeOrder();
    preparedIngredients.clear();
    currentStep = ProcessStep::Idle;
    if (orderManager.hasActiveOrder())
        currentStep = ProcessStep::PreparingIngredients;
}

bool BurgerFactoryModel::consumeIngredients()
{
    BurgerRecipe recipe = getRecipe(orderManager.getCurrentOrder().type);
    for (const auto& [ingredient, amount] : recipe.ingredients)
        if (!inventoryManager.hasEnough(ingredient, amount)) return false;
    preparedIngredients.clear();
    for (const auto& [ingredient, amount] : recipe.ingredients)
    {
        inventoryManager.use(ingredient, amount);
        preparedIngredients[ingredient] = amount;
    }
    return true;
}

bool BurgerFactoryModel::checkQuality() const
{
    BurgerRecipe recipe = getRecipe(orderManager.getCurrentOrder().type);
    if (preparedIngredients.size() != recipe.ingredients.size()) return false;
    for (const auto& [ingredient, amount] : recipe.ingredients)
    {
        auto it = preparedIngredients.find(ingredient);
        if (it == preparedIngredients.end() || it->second != amount) return false;
    }
    return true;
}

ProcessStep  BurgerFactoryModel::getCurrentStep()          const { return currentStep; }
int          BurgerFactoryModel::getTotalBurgersProduced() const { return orderManager.getCompletedCount(); }
int          BurgerFactoryModel::getMoney()                const { return moneyManager.getMoney(); }
bool         BurgerFactoryModel::hasOrder()                const { return orderManager.hasOrder(); }
const Order& BurgerFactoryModel::getCurrentOrder()         const { return orderManager.getCurrentOrder(); }
Machine*     BurgerFactoryModel::getMachine(ProcessStep step)    { return productionLine.getMachine(step); }
int          BurgerFactoryModel::getIngredientAmount(IngredientType type) const { return inventoryManager.getAmount(type); }
bool         BurgerFactoryModel::isQualityCheckPassed()    const { return qualityCheckPassed; }
bool         BurgerFactoryModel::isCurrentMachineFailed()  const { return productionLine.isMachineFailed(currentStep); }
bool         BurgerFactoryModel::isCurrentMachinePaused()  const { return productionLine.isMachinePaused(currentStep); }
const std::map<IngredientType, int>& BurgerFactoryModel::getPreparedIngredients() const { return preparedIngredients; }
void         BurgerFactoryModel::refillInventory()               { inventoryManager.refill(); }
const std::vector<Order>& BurgerFactoryModel::getCompletedOrders() const { return orderManager.getCompletedOrders(); }
