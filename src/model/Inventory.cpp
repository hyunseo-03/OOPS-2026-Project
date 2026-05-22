#include "model/Inventory.h"

Inventory::Inventory()
  : buns(50),
    patties(40),
    lettuce(50),
    tomato(40),
    cheese(30),
    sauces(60)
{
}

bool Inventory::hasIngredients() const
{
  return buns > 0 &&
        patties > 0 &&
        lettuce > 0 &&
        tomato > 0 &&
        cheese > 0 &&
        sauces > 0;
}

void Inventory::useIngredients()
{
  if (!hasIngredients())
      return;

  buns--;
  patties--;
  lettuce--;
  tomato--;
  cheese--;
  sauces--;
}

void Inventory::addIngredients()
{
  buns += 10;
  patties += 10;
  lettuce += 10;
  tomato += 10;
  cheese += 10;
  sauces += 10;
}

int Inventory::getBuns() const
{
  return buns;
}

int Inventory::getPatties() const
{
  return patties;
}

int Inventory::getLettuce() const
{
  return lettuce;
}

int Inventory::getTomato() const
{
  return tomato;
}

int Inventory::getCheese() const
{
  return cheese;
}

int Inventory::getSauces() const
{
  return sauces;
}