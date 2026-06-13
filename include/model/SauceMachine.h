#pragma once
#include "model/Machine.h"

class SauceMachine : public Machine
{
private:
    bool sauceApplied;

public:
    SauceMachine();
    void update(float dt) override;

    bool isSauceApplied() const;
};
