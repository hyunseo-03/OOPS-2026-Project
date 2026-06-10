#include "model/QualityChecker.h"

QualityChecker::QualityChecker() : Machine("QualityChecker", 2.0f) {}

void QualityChecker::update(float dt)
{
    checkMalfunction(dt);
    if (!running || paused) return;

    progress += dt / cycleTime;
    if (progress >= 1.0f) { progress = 1.0f; stop(); }
}

bool QualityChecker::inspect(const std::map<IngredientType, int>& preparedIngredients,
                             const BurgerRecipe& recipe) const
{
    if (preparedIngredients.size() != recipe.ingredients.size()) return false;

    for (const auto& [ingredient, amount] : recipe.ingredients)
    {
        auto it = preparedIngredients.find(ingredient);
        if (it == preparedIngredients.end() || it->second != amount) return false;
    }

    return true;
}
