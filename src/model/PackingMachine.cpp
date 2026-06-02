#include "model/PackingMachine.h"

static std::string toLabel(BurgerType type)
{
    switch (type)
    {
        case BurgerType::CLASSIC: return "Classic Burger";
        case BurgerType::CHEESE:  return "Cheese Burger";
        case BurgerType::DOUBLE:  return "Double Burger";
        default:                  return "Unknown";
    }
}

PackingMachine::PackingMachine()
    : Machine("Packing Machine", 2.0f, 0.02f), sealed(false) {}

void PackingMachine::configure(const MachineConfig& config)
{
    burgerLabel = toLabel(config.burgerType);
    sealed      = false;
}

void PackingMachine::update(float dt)
{
    checkMalfunction(dt);
    if (!running || paused) return;

    progress += dt / cycleTime;
    if (progress >= 1.0f)
    {
        progress = 1.0f;
        sealed   = true;
        stop();
    }
}

const std::string& PackingMachine::getBurgerLabel() const { return burgerLabel; }
bool               PackingMachine::isSealed()       const { return sealed; }
