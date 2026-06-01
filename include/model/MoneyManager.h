#pragma once

// ─────────────────────────────────────────────
// MoneyManager - 돈 관리 클래스
// ─────────────────────────────────────────────
class MoneyManager {
private:
    int money;

public:
    MoneyManager();

    void add(int amount);
    bool spend(int amount);
    int  getMoney() const;
};
