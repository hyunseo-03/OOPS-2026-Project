#include "QualityChecker.h"

QualityChecker::QualityChecker()
    : Machine("Quality Checker", 1.5f) {}

void QualityChecker::update(float dt) {
    if (running) {
        progress += dt / cycleTime;
        if (progress >= 1.0f) {
            progress = 1.0f;
            stop();
        }
    }
}