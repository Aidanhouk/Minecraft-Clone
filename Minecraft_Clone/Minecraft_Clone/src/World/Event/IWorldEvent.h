#pragma once

class World;

struct IWorldEvent {
    virtual ~IWorldEvent() = default;
    virtual void handle(World &world) = 0;
};