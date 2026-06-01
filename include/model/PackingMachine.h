#pragma once
#include "model/Machine.h"

class PackingMachine : public Machine {
public:
    PackingMachine();
    void update(float dt) override;
};
