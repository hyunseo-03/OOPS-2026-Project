#pragma once
#include <string>
#include "model/Machine.h"

class PackingMachine : public Machine
{
private:
    std::string burgerLabel;
    bool        sealed;

public:
    PackingMachine();
    void configure(const MachineConfig& config) override;  // burgerType 읽기
    void update(float dt) override;

    const std::string& getBurgerLabel() const;
    bool               isSealed()       const;
};
