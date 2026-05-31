#include "BurgerFactoryModel.h"


BurgerFactoryModel::BurgerFactoryModel()
    : currentStep(ProcessStep::Idle), totalBurgersProduced(0)
{
}

void BurgerFactoryModel::update(float dt)
{
    if (currentStep == ProcessStep::Done) {
        totalBurgersProduced += 1; 
        currentStep = ProcessStep::Idle;
    }
}

void BurgerFactoryModel::startProcess(ProcessStep step)
{
    if (canProceed(step)) {
        currentStep = step;
    }
}

bool BurgerFactoryModel::canProceed(ProcessStep step)
{
    return static_cast<int>(step) == static_cast<int>(currentStep) + 1;
}

void BurgerFactoryModel::nextStep()
{
    if (currentStep != ProcessStep::Done) {
        currentStep = static_cast<ProcessStep>(static_cast<int>(currentStep) + 1);
    }
}
