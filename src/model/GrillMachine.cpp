#include "model/GrillMachine.h"

GrillMachine::GrillMachine()
    : Machine("Grill Machine", BASE_CYCLE, 0.04f),
      pattyCount(1), pattiesGrilled(0) {}

void GrillMachine::configure(const MachineConfig& config)
{
    pattyCount     = config.pattyCount;
    pattiesGrilled = 0;
}

void GrillMachine::update(float dt)
{
    checkMalfunction(dt);
    if (!running || paused) return;

    progress += dt / BASE_CYCLE;

    if (progress >= 1.0f)
    {
        pattiesGrilled++;

        if (pattiesGrilled < pattyCount)
        {
            // 아직 남은 패티 있음 → 다음 패티 시작
            progress = 0.0f;
        }
        else
        {
            // 모든 패티 완료
            progress = 1.0f;
            stop();
        }
    }
}

int GrillMachine::getPattyCount()     const { return pattyCount; }
int GrillMachine::getPattiesGrilled() const { return pattiesGrilled; }
