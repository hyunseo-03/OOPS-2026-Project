#pragma once
#include "model/Machine.h"

class SauceMachine : public Machine {
public:
    SauceMachine();
    void update(float dt) override;
};
