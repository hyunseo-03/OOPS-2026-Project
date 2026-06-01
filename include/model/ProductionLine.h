#pragma once
#include <vector>
#include <memory>
#include "model/Machine.h"
#include "ProcessStep.h"


class ProductionLine {
private:
    std::vector<std::unique_ptr<Machine>> machines;

    // ProcessStep → machines 배열 인덱스 변환 헬퍼
    int stepToIndex(ProcessStep step) const;

public:
    ProductionLine();

    void update(float dt);
    void startMachine(ProcessStep step);
    bool isMachineDone(ProcessStep step) const;
    Machine* getMachine(ProcessStep step);
};
