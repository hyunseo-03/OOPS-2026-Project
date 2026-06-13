#pragma once
#include "model/Machine.h"

class AssemblyMachine : public Machine
{
private:
    bool assembled;

public:
    AssemblyMachine();
    void update(float dt) override;

    bool isAssembled() const;
};
