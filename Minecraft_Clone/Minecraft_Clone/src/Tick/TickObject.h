#pragma once

class TickObject
{

public:
    TickObject(unsigned int tickRate = 0u)
        : tickRate(tickRate) {}

    virtual ~TickObject() = default;
    virtual void tickUpdate(unsigned int tickTime) = 0;

protected:
    const unsigned int tickRate;
    unsigned int currentTick = 0u;
};