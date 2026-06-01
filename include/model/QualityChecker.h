#pragma once
#include "model/Machine.h"

class QualityChecker : public Machine {
public:
    QualityChecker();
    void update(float dt) override;
};
