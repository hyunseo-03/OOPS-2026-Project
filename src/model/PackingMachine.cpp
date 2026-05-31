#include "../include/model/PackingMachine.h"

PackingMachine::PackingMachine()
    : Machine("Packing Machine", 2.0f)
{
}

void PackingMachine::update(float dt)
{
    if (running) {
        progress += dt / cycleTime;
        if (progress >= 1.0f) {
            progress = 1.0f; 
            stop(); 
        }
    }
}