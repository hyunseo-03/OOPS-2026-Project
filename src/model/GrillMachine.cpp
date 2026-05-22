#include "../include/model/GrillMachine.h"

GrillMachine::GrillMachine()
    : Machine("Grill Machine", 3.0f)
{
}

void GrillMachine::update(float dt)
{
    Machine::update(dt);
}