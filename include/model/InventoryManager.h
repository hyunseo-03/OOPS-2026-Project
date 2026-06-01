#pragma once
#include <map>
#include "IngredientType.h"

// ─────────────────────────────────────────────
// InventoryManager - 재고 관리 클래스
// IngredientType → 수량 map으로 재고를 관리한다.
// ─────────────────────────────────────────────
class InventoryManager {
private:
    std::map<IngredientType, int> inventory;

public:
    InventoryManager();

    bool hasEnough(IngredientType type, int amount) const;
    bool use(IngredientType type, int amount);
    void add(IngredientType type, int amount);
    int  getAmount(IngredientType type) const;

    // 초기 재고 세팅 (게임 시작 시 호출)
    void refill();
};
