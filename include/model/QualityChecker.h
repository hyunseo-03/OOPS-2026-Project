#pragma once
#include <map>
#include "model/Machine.h"
#include "model/BurgerRecipe.h"
#include "enum/IngredientType.h"

class QualityChecker : public Machine {
public:
    QualityChecker();
    void update(float dt) override;
    bool inspect(const std::map<IngredientType, int>& preparedIngredients,
                 const BurgerRecipe& recipe) const;
};
