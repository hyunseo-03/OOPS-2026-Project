#include "MoneyManager.h"

MoneyManager::MoneyManager() : money(0) {}

void MoneyManager::add(int amount) {
    money += amount;
  }

bool MoneyManager::spend(int amount) {
  if (money >= amount) {
    money -= amount;
    return true;
  }
  return false; 
}
int MoneyManager::getMoney() const {
  return money;
}