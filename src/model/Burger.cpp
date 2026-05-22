#include "../include/model/Burger.h"

Burger::Burger()
  : prepared(false),
    grilled(false),
    sauced(false),
    packed(false)
{}

void Burger::setPrepared(bool value)
{
  prepared = value;
}

void Burger::setGrilled(bool value)
{
  grilled = value;
}

void Burger::setSauced(bool value)
{
  sauced = value;
}

void Burger::setPacked(bool value)
{
  packed = value;
}

bool Burger::isPrepared() const
{
  return prepared;
}

bool Burger::isGrilled() const
{
  return grilled;
}

bool Burger::isSauced() const
{
  return sauced;
}

bool Burger::isPacked() const
{
  return packed;
}

bool Burger::isComplete() const
{
  return prepared && grilled && sauced && packed;
}

void Burger::reset()
{
  prepared = false;
  grilled = false;
  sauced = false;
  packed = false;
}