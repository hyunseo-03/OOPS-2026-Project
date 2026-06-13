#pragma once
#include "model/Machine.h"

class GrillMachine : public Machine
{
private:
    int pattyCount;
    int pattiesGrilled;

    static constexpr float BASE_CYCLE = 3.0f;

public:
    GrillMachine();
    void configure(const MachineConfig& config) override;
    void update(float dt) override;

    int getPattyCount()     const;
    int getPattiesGrilled() const;
};
