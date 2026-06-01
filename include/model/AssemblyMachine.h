#pragma once
#include "model/Machine.h"

class AssemblyMachine : public Machine {
public:
    AssemblyMachine();
    void update(float dt) override;
};
