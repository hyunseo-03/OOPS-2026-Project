#include "../include/model/PackingMachine.h"

PackingMachine::PackingMachine()
    : Machine("Packing Machine", 2.0f)
{
}

void PackingMachine::update(float dt)
{
    Machine::update(dt);
}