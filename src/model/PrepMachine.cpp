#include "model/PrepMachine.h"

void PrepMachine::update(float dt)
{
    checkMalfunction(dt);
    if (!running || paused) return;

    progress += dt / cycleTime;
    if (progress >= 1.0f) { progress = 1.0f; stop(); }
}
