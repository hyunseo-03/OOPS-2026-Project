#include "model/MoneyManager.h"

MoneyManager::MoneyManager() : money(500) {}  // 시작 자금 500

void MoneyManager::add(int amount)
{
    money += amount;
}

bool MoneyManager::spend(int amount)
{
    if (money < amount) return false;
    money -= amount;
    return true;
}

int MoneyManager::getMoney() const
{
    return money;
}
