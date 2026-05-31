#include "AssemblyMachine.h"

AssemblyMachine::AssemblyMachine() 
  : Machine("Assembly Machine", 4.0f) {}

void AssemblyMachine::update(float dt) {
    if (running) {
        progress += dt / cycleTime;
        if (progress >= 1.0f) {
            progress = 1.0f; 
            stop(); 
        }
    }
}

