#pragma once
#include <vector>
#include <memory>
#include "Machine.h"
#include "ProcessStep.h"

class ProductionLine {
private:
  std::vector<std::unique_ptr<Machine>> machines; 
public:
  void update(float dt);
  void startMachine(ProcessStep step);
  bool isMachineDone(ProcessStep step) const;
  Machine* getMachine(ProcessStep step);
};