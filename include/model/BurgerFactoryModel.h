#pragma once

#include "ProcessStep.h"


class BurgerFactoryModel
{
private:
    ProcessStep currentStep;
    int totalBurgersProduced;

public:
    BurgerFactoryModel();
    void update(float dt);
    void startProcess(ProcessStep step);
    bool canProceed(ProcessStep step);
    void nextStep();
    ProcessStep getCurrentStep() const;
    int getTotalBurgersProduced() const;
};
