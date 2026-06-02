#pragma once
#include <vector>
#include "model/Machine.h"
#include "IngredientType.h"

class AssemblyMachine : public Machine
{
private:
    std::vector<IngredientType> toAssemble;
    std::vector<IngredientType> assembled;
    float stepTimer;
    static constexpr float STEP_TIME = 0.8f;

public:
    AssemblyMachine();
    void configure(const MachineConfig& config) override;  // assemblyOrder 읽기
    void update(float dt) override;

    const std::vector<IngredientType>& getAssembled()  const;
    const std::vector<IngredientType>& getToAssemble() const;
};
