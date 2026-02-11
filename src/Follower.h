#ifndef FOLLOWER_H
#define FOLLOWER_H

#include "Entity.h"

class Follower2D {
public:

    const Entity2D* target = nullptr;

    explicit Follower2D(const Entity2D* target) : target(target) {}

    virtual void follow(float deltaTime, float cameraSpeed = 3.5f, bool fixed = false) = 0;
};

#endif // FOLLOWER_H
