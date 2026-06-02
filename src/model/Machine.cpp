#include "model/Machine.h"
#include <cstdlib>

Machine::Machine(const std::string& name, float cycleTime, float malfunctionRate)
    : name(name), running(false), paused(false), progress(0.0f),
      cycleTime(cycleTime), failed(false), malfunctionRate(malfunctionRate)
{}

void Machine::checkMalfunction(float dt)
{
    if (!running || paused || failed) return;
    float roll = static_cast<float>(rand()) / RAND_MAX;
    if (roll < malfunctionRate * dt)
    {
        running = false;
        failed  = true;
    }
}

void Machine::start()
{
    if (failed) return;
    running  = true;
    paused   = false;
    progress = 0.0f;
}

void Machine::stop()    { running = false; }
void Machine::reset()   { running = false; paused = false; progress = 0.0f; failed = false; }
void Machine::repair()  { failed = false; running = false; paused = false; progress = 0.0f; }
void Machine::pause()   { if (running) paused = true; }
void Machine::resume()  { if (paused) paused = false; }

bool  Machine::isDone()    const { return !failed && progress >= 1.0f; }
bool  Machine::isFailed()  const { return failed; }
bool  Machine::isRunning() const { return running && !paused; }
bool  Machine::isPaused()  const { return paused; }
float Machine::getProgress()     const { return progress; }
const std::string& Machine::getName() const { return name; }
