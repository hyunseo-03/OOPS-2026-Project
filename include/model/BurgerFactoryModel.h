#pragma once

#include "model/Burger.h"
#include "model/Inventory.h"
#include "model/PrepMachine.h"
#include "model/GrillMachine.h"
#include "model/SauceMachine.h"
#include "model/PackingMachine.h"

enum class ProcessStep {
    Idle,
    Preparing,
    Grilling,
    Saucing,
    Packing,
    Done
};

class BurgerFactoryModel {
private:
    Burger burger;
    Inventory inventory;

    PrepMachine prepMachine;
    GrillMachine grillMachine;
    SauceMachine sauceMachine;
    PackingMachine packingMachine;

    ProcessStep currentStep;

    int money;
    int burgerCount;
    bool producing;

public:
    BurgerFactoryModel();

    void startProduction();
    void stopProduction();
    void reset();
    void update(float dt);
    void addIngredients();

    int getMoney() const;
    int getBurgerCount() const;
    bool isProducing() const;
    ProcessStep getCurrentStep() const;

    const Burger& getBurger() const;
    const Inventory& getInventory() const;

    const PrepMachine& getPrepMachine() const;
    const GrillMachine& getGrillMachine() const;
    const SauceMachine& getSauceMachine() const;
    const PackingMachine& getPackingMachine() const;
};