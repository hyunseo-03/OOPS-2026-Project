#pragma once
#include "imgui.h"
#include "model/IFactoryViewData.h"
#include "controller/FactoryController.h"

class FactoryView
{
private:
    IFactoryViewData& model;
    FactoryController&  controller;

    const char* stepToString(ProcessStep step) const;

public:
    FactoryView(IFactoryViewData& model, FactoryController& controller);

    void render();
    void renderOrderHistory();
};
