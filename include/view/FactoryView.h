#pragma once
#include "imgui.h"
#include "model/BurgerFactoryModel.h"
#include "controller/FactoryController.h"

class FactoryView
{
private:
    BurgerFactoryModel& model;
    FactoryController&  controller;

    const char* stepToString(ProcessStep step) const;

public:
    FactoryView(BurgerFactoryModel& model, FactoryController& controller);

    void render();
    void renderOrderHistory();
};
