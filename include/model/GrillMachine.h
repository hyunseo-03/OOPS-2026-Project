#pragma once
#include "model/Machine.h"

class GrillMachine : public Machine
{
private:
    int   pattyCount;
    int   pattiesGrilled;
    float grillTemperature;
    static constexpr float TARGET_TEMP = 180.0f;
    static constexpr float BASE_CYCLE  = 3.0f;

public:
    GrillMachine();
    void configure(const MachineConfig& config) override;  // pattyCount 읽기
    void update(float dt) override;

    float getGrillTemperature() const;
    int   getPattyCount()       const;
    int   getPattiesGrilled()   const;
};
