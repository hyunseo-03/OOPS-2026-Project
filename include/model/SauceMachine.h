#pragma once
#include "model/Machine.h"

class SauceMachine : public Machine
{
private:
    int   targetAmount;
    float dispensed;

public:
    SauceMachine();
    void configure(const MachineConfig& config) override;  // sauceAmount 읽기
    void update(float dt) override;

    int getTargetAmount() const;
    int getDispensed()    const;
};
