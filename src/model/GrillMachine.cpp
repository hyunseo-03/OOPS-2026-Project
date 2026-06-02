#include "model/GrillMachine.h"

GrillMachine::GrillMachine()
    : Machine("Grill Machine", BASE_CYCLE, 0.04f),
      pattyCount(1), pattiesGrilled(0), grillTemperature(20.0f) {}

void GrillMachine::configure(const MachineConfig& config)
{
    pattyCount     = config.pattyCount;
    pattiesGrilled = 0;
    grillTemperature = 20.0f;
}

void GrillMachine::update(float dt)
{
    checkMalfunction(dt);
    if (!running || paused) return;

    grillTemperature += (TARGET_TEMP - 20.0f) * (dt / BASE_CYCLE);
    if (grillTemperature > TARGET_TEMP) grillTemperature = TARGET_TEMP;

    progress += dt / BASE_CYCLE;
    if (progress >= 1.0f)
    {
        pattiesGrilled++;
        if (pattiesGrilled < pattyCount)
        {
            progress         = 0.0f;
            grillTemperature = 20.0f;  // 다음 패티 시작
        }
        else
        {
            progress = 1.0f;
            stop();
        }
    }
}

float GrillMachine::getGrillTemperature() const { return grillTemperature; }
int   GrillMachine::getPattyCount()       const { return pattyCount; }
int   GrillMachine::getPattiesGrilled()   const { return pattiesGrilled; }
