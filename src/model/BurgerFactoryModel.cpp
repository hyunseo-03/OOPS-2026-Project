#include "model/BurgerFactoryModel.h"

BurgerFactoryModel::BurgerFactoryModel()
    : currentStep(ProcessStep::Idle),
      money(0),
      burgerCount(0),
      producing(false)
{
}

void BurgerFactoryModel::startProduction()
{
    if (producing)
        return;

    if (!inventory.hasIngredients())
        return;

    burger.reset();
    inventory.useIngredients();

    producing = true;
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
}

void BurgerFactoryModel::reset()
{
    burger.reset();

    prepMachine.reset();
    grillMachine.reset();
    sauceMachine.reset();
    packingMachine.reset();

    currentStep = ProcessStep::Idle;
    producing = false;

    money = 0;
    burgerCount = 0;
}

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
            money += 100;

            currentStep = ProcessStep::Done;
            producing = false;
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

int BurgerFactoryModel::getMoney() const
{
    return money;
}

int BurgerFactoryModel::getBurgerCount() const
{
    return burgerCount;
}

bool BurgerFactoryModel::isProducing() const
{
    return producing;
}

ProcessStep BurgerFactoryModel::getCurrentStep() const
{
    return currentStep;
}

const Burger& BurgerFactoryModel::getBurger() const
{
    return burger;
}

const Inventory& BurgerFactoryModel::getInventory() const
{
    return inventory;
}

const PrepMachine& BurgerFactoryModel::getPrepMachine() const
{
    return prepMachine;
}

const GrillMachine& BurgerFactoryModel::getGrillMachine() const
{
    return grillMachine;
}

const SauceMachine& BurgerFactoryModel::getSauceMachine() const
{
    return sauceMachine;
}

const PackingMachine& BurgerFactoryModel::getPackingMachine() const
{
    return packingMachine;
}