#pragma once

class TickObject{
public:
    virtual ~TickObject() = default;
    virtual void tickUpdate(unsigned int tickTime) = 0;
};