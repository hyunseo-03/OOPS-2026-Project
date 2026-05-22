#pragma once

#include <string>

class Machine {
private:
    std::string name;
    bool running;
    float progress;
    float cycleTime;

public:
    Machine(const std::string& name, float cycleTime);
    virtual ~Machine() = default;

    virtual void update(float dt);

    void start();
    void stop();
    void reset();

    bool isRunning() const;
    bool isDone() const;

    float getProgress() const;
    float getCycleTime() const;
    std::string getName() const;
};