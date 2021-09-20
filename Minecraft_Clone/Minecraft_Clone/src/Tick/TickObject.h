#ifndef TICK_OBJECT_H
#define TICK_OBJECT_H

class TickObject{
public:
    virtual ~TickObject() = default;
    virtual void tickUpdate(unsigned int tickTime) = 0;
};

#endif
