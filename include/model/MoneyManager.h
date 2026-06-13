#pragma once

class MoneyManager {
private:
    int money;

public:
    MoneyManager();

    void add(int amount);
    bool spend(int amount);
    int  getMoney() const;
};
