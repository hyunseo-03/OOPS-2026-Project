#include "controller/FactoryController.h"

FactoryController::FactoryController(BurgerFactoryModel& model) : model(model) {}

void FactoryController::update(float dt)                  { model.update(dt); }
void FactoryController::onStartMachine(ProcessStep step)  { model.startProcess(step); }
void FactoryController::onNewOrder(BurgerType type)       { model.addOrder(type); }
void FactoryController::onPackBurger()                    { model.packBurger(); }
void FactoryController::onRefillInventory()               { model.refillInventory(); }
void FactoryController::onRepairMachine(ProcessStep step) { model.repairMachine(step); }
void FactoryController::onTogglePause(ProcessStep step)   { model.togglePauseMachine(step); }
