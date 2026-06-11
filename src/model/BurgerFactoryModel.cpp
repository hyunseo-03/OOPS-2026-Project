#include "model/BurgerFactoryModel.h"
#include "model/BurgerRecipe.h"

BurgerFactoryModel::BurgerFactoryModel()
    : currentStep(ProcessStep::Idle), qualityCheckPassed(false),
      productionTimerRunning(false), gameOver(false), statusMessage("") {}

void BurgerFactoryModel::update(float dt)
{
    if (gameOver) return;

    // Start Production Line을 누른 뒤부터 평점용 시간을 누적한다.
    if (productionTimerRunning && hasOrder() && orderManager.hasActiveOrder()) {
        orderManager.updateActiveOrderTime(dt); 
    }

    if (currentStep == ProcessStep::Idle || currentStep == ProcessStep::Done) return;

    productionLine.update(dt);

    if (productionLine.isMachineFailed(currentStep) && moneyManager.getMoney() < REPAIR_COST)
    {
        gameOver = true;
        productionTimerRunning = false;
        statusMessage = "GAME OVER: Machine broke down and you do not have enough money to repair it.";
        return;
    }

    if (productionLine.isMachineDone(currentStep))
        nextStep();
}

void BurgerFactoryModel::handleMalfunction()
{
    // 고장 감지: 단계는 그대로 유지 (수리 후 재시작 가능하도록)
}

bool BurgerFactoryModel::repairMachine(ProcessStep step)
{
    if (!productionLine.isMachineFailed(step)) return false;
    if (!moneyManager.spend(REPAIR_COST))
    {
        statusMessage = "Not enough money to repair the machine.";
        if (step == currentStep)
        {
            gameOver = true;
            productionTimerRunning = false;
            statusMessage = "GAME OVER: Machine broke down and you do not have enough money to repair it.";
        }
        return false;
    }
    productionLine.repairMachine(step);
    statusMessage = "Machine repaired.";
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
    if (gameOver) return;
    if (step != currentStep) return;
    if (productionLine.isMachineFailed(step)) return;
    Machine* machine = productionLine.getMachine(step);
    if (machine && (machine->isRunning() || machine->isPaused() || machine->isDone())) return;

    if (step == ProcessStep::PreparingIngredients)
    {
        if (!consumeIngredients())
        {
            if (moneyManager.getMoney() < REFILL_COST)
            {
                gameOver = true;
                productionTimerRunning = false;
                statusMessage = "GAME OVER: Not enough ingredients and not enough money to refill inventory.";
            }
            else
            {
                statusMessage = "Not enough ingredients. Refill inventory to continue.";
            }
            return;
        }
    }

    productionLine.startMachine(step);
    productionTimerRunning = true;
    statusMessage = "";
}

bool BurgerFactoryModel::canProceed(ProcessStep step) const
{
    if (gameOver) return false;
    if (step != currentStep) return false;
    if (!orderManager.hasActiveOrder()) return false;
    if (productionLine.isMachineFailed(step)) return false;
    const Machine* machine = productionLine.getMachine(step);
    if (machine && (machine->isRunning() || machine->isPaused() || machine->isDone())) return false;

    if (step == ProcessStep::PreparingIngredients)
    {
        BurgerRecipe recipe = getRecipe(orderManager.getCurrentOrder().type);
        for (const auto& [ingredient, amount] : recipe.ingredients)
            if (!inventoryManager.hasEnough(ingredient, amount)) return false;
    }
    return true;
}

void BurgerFactoryModel::enterStep(ProcessStep step, bool autoStart, const MachineConfig* config)
{
    currentStep = step;

    if (step == ProcessStep::Idle || step == ProcessStep::Done)
        return;

    productionLine.resetMachine(step);

    if (config)
        productionLine.configureMachine(step, *config);

    if (autoStart)
        productionLine.startMachine(step);
}

void BurgerFactoryModel::nextStep()
{
    BurgerType   type   = orderManager.getCurrentOrder().type;
    BurgerRecipe recipe = getRecipe(type);
    MachineConfig config;
    config.burgerType = type;

    switch (currentStep)
    {
        case ProcessStep::Idle:
            enterStep(ProcessStep::PreparingIngredients);
            break;

        case ProcessStep::PreparingIngredients:
            config.pattyCount = recipe.ingredients.count(IngredientType::PATTY)
                              ? recipe.ingredients.at(IngredientType::PATTY) : 1;
            enterStep(ProcessStep::GrillPatty, true, &config);
            break;

        case ProcessStep::GrillPatty:
            enterStep(ProcessStep::AddSauce, true);
            break;

        case ProcessStep::AddSauce:
            enterStep(ProcessStep::AssembleBurger, true);
            break;

        case ProcessStep::AssembleBurger:
            enterStep(ProcessStep::QualityCheck, true);
            break;

        case ProcessStep::QualityCheck:
            qualityCheckPassed = checkQuality();
            if (qualityCheckPassed)
            {
                enterStep(ProcessStep::PackBurger, true, &config);
            }
            else
            {
                preparedIngredients.clear();
                orderManager.completeOrder();
                productionTimerRunning = false;
                if (orderManager.hasActiveOrder())
                    enterStep(ProcessStep::PreparingIngredients);
                else
                    enterStep(ProcessStep::Idle);
            }
            break;

        case ProcessStep::PackBurger:
            {
                BurgerRecipe r = getRecipe(orderManager.getCurrentOrder().type);
                moneyManager.add(r.price);
                orderManager.completeOrder();
                productionTimerRunning = false;
                preparedIngredients.clear();
                qualityCheckPassed = false;
                if (orderManager.hasActiveOrder())
                    enterStep(ProcessStep::PreparingIngredients);
                else
                    enterStep(ProcessStep::Idle);
            }
            break;

        case ProcessStep::Done:
            enterStep(ProcessStep::Idle);
            break;
    }
}

void BurgerFactoryModel::addOrder(BurgerType type)
{
    if (gameOver) return;
    orderManager.addOrder(type);
    statusMessage = "";
    if (currentStep == ProcessStep::Idle)
        enterStep(ProcessStep::PreparingIngredients);
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
    return productionLine.inspectQuality(preparedIngredients, recipe);
}

ProcessStep  BurgerFactoryModel::getCurrentStep()          const { return currentStep; }
int          BurgerFactoryModel::getTotalBurgersProduced() const { return orderManager.getCompletedCount(); }
int          BurgerFactoryModel::getMoney()                const { return moneyManager.getMoney(); }
bool         BurgerFactoryModel::hasOrder()                const { return orderManager.hasOrder(); }
const Order& BurgerFactoryModel::getCurrentOrder()         const { return orderManager.getCurrentOrder(); }
int          BurgerFactoryModel::getIngredientAmount(IngredientType type) const { return inventoryManager.getAmount(type); }
bool         BurgerFactoryModel::isCurrentMachineFailed()  const { return productionLine.isMachineFailed(currentStep); }
bool         BurgerFactoryModel::isCurrentMachinePaused()  const { return productionLine.isMachinePaused(currentStep); }
bool         BurgerFactoryModel::isGameOver()              const { return gameOver; }
const std::string& BurgerFactoryModel::getStatusMessage()  const { return statusMessage; }
void BurgerFactoryModel::refillInventory()
{
    if (gameOver) return;

    if (!moneyManager.spend(REFILL_COST))
    {
        statusMessage = "Not enough money to refill inventory.";
        return;
    }

    inventoryManager.refill();
    statusMessage = "Inventory refilled.";
}
const std::vector<Order>& BurgerFactoryModel::getCompletedOrders() const { return orderManager.getCompletedOrders(); }
const std::vector<Order>& BurgerFactoryModel::getQueuedOrders() const { return orderManager.getQueuedOrders(); }
int BurgerFactoryModel::getRepairCost() const { return REPAIR_COST; }

bool BurgerFactoryModel::upgradeMachine(ProcessStep step)
{
    if (gameOver) return false;

    Machine* machine = productionLine.getMachine(step);
    if (!machine) return false;

    if (machine->getLevel() >= machine->getMaxLevel()) {
        return false; // 이미 만렙
    }

    int cost = machine->getUpgradeCost();
    
    // moneyManager에서 돈 차감 시도
    if (moneyManager.spend(cost)) {
        machine->upgrade();
        statusMessage = "Machine upgraded.";
        return true; // 업그레이드 성공
    }
    
    statusMessage = "Not enough money to upgrade this machine.";
    return false; // 돈 부족
}
float BurgerFactoryModel::getAverageReputation() const
{
    return orderManager.getAverageReputation();
}

bool BurgerFactoryModel::hasMachine(ProcessStep step) const
{
    return productionLine.getMachine(step) != nullptr;
}

bool BurgerFactoryModel::isMachineRunning(ProcessStep step) const
{
    const Machine* machine = productionLine.getMachine(step);
    return machine && machine->isRunning();
}

bool BurgerFactoryModel::isMachinePaused(ProcessStep step) const
{
    return productionLine.isMachinePaused(step);
}

bool BurgerFactoryModel::isMachineFailed(ProcessStep step) const
{
    return productionLine.isMachineFailed(step);
}

bool BurgerFactoryModel::isMachineDone(ProcessStep step) const
{
    return productionLine.isMachineDone(step);
}

float BurgerFactoryModel::getMachineProgress(ProcessStep step) const
{
    const Machine* machine = productionLine.getMachine(step);
    return machine ? machine->getProgress() : 0.0f;
}

const std::string& BurgerFactoryModel::getMachineName(ProcessStep step) const
{
    static const std::string emptyName = "";
    const Machine* machine = productionLine.getMachine(step);
    return machine ? machine->getName() : emptyName;
}

int BurgerFactoryModel::getMachineLevel(ProcessStep step) const
{
    const Machine* machine = productionLine.getMachine(step);
    return machine ? machine->getLevel() : 0;
}

int BurgerFactoryModel::getMachineMaxLevel(ProcessStep step) const
{
    const Machine* machine = productionLine.getMachine(step);
    return machine ? machine->getMaxLevel() : 0;
}

int BurgerFactoryModel::getMachineUpgradeCost(ProcessStep step) const
{
    const Machine* machine = productionLine.getMachine(step);
    return machine ? machine->getUpgradeCost() : 0;
}

float BurgerFactoryModel::getMachineCycleTime(ProcessStep step) const
{
    const Machine* machine = productionLine.getMachine(step);
    return machine ? machine->getCycleTime() : 0.0f;
}

float BurgerFactoryModel::getMachineMalfunctionRate(ProcessStep step) const
{
    const Machine* machine = productionLine.getMachine(step);
    return machine ? machine->getMalfunctionRate() : 0.0f;
}
