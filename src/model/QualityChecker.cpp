#include "model/QualityChecker.h"

QualityChecker::QualityChecker() : Machine("QualityChecker", 2.0f) {}

void QualityChecker::update(float dt)
{
    checkMalfunction(dt);
    if (!running || paused) return;

    progress += dt / cycleTime;
    if (progress >= 1.0f) { progress = 1.0f; stop(); }
}
