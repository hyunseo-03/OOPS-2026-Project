#pragma once

#include "../model/Machine.h"

class PrepMachine : public Machine {
public:
    PrepMachine();

    void update(float dt) override;
};