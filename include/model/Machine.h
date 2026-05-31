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
    
    void start();
    void stop();

    virtual void update(float dt);

    bool isDone() const;
    float getProgress() const;
    const std::string& getName() const;
};
