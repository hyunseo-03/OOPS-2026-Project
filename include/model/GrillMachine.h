#pragma once
#include "model/Machine.h"

class GrillMachine : public Machine {
public:
    GrillMachine();
    void update(float dt) override;
};
