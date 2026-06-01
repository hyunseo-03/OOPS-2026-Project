#pragma once
#include <string>


class Machine
{
protected:
    std::string name;
    bool running;
    float progress;   
    float cycleTime;  

public:
    Machine(const std::string& name, float cycleTime);
    virtual ~Machine() = default;

    void start();
    void stop();
    void reset();


    virtual void update(float dt) = 0;

    bool isDone() const;
    float getProgress() const;
    const std::string& getName() const;
    bool isRunning() const;
};
