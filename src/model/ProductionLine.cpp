#include "ProductionLine.h"

void ProductionLine::update(float dt) {
  for (auto& machine : machines) {
    machine->update(dt);
  }
}

void ProductionLine::startMachine(ProcessStep step) {
  if (step == ProcessStep::Idle || step == ProcessStep::Done) return;
  int index = static_cast<int>(step) - 1; 
  if (index >= 0 && index < machines.size()) {
    machines[index]->start();
  }
}

bool ProductionLine::isMachineDone(ProcessStep step) const {
  if (step == ProcessStep::Idle || step == ProcessStep::Done) return false;
  int index = static_cast<int>(step) - 1; 
  if (index >= 0 && index < machines.size()) {
    return machines[index]->isDone();
  }
  return false;
}

Machine* ProductionLine::getMachine(ProcessStep step) {
  if (step == ProcessStep::Idle || step == ProcessStep::Done) return nullptr;
  int index = static_cast<int>(step) - 1; 
  if (index >= 0 && index < machines.size()) {
    return machines[index].get();
  }
  return nullptr;
}                                                          