#include "../include/model/PrepMachine.h"

PrepMachine::PrepMachine()
    : Machine("Prep Machine", 2.0f)
{
}

void PrepMachine::update(float dt)
{
    Machine::update(dt);
}