#ifndef FOLLOWER_H
#define FOLLOWER_H

#include "Entity.hh"

class Follower2D {
public:
    virtual ~Follower2D() = default;

    const Entity2D* target = nullptr;

    explicit Follower2D(const Entity2D* target) : target(target) {}

    virtual void follow(float deltaTime, float cameraSpeed) = 0;
};

#endif // FOLLOWER_H
