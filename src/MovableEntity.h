#ifndef MOVABLEENTITY_H
#define MOVABLEENTITY_H

#include "Entity.h"
#include <SFML/Graphics.hpp>

class MovableEntity2D : public Entity2D {
public:

    sf::Vector2f velocity;
    sf::Vector2f acceleration;

    MovableEntity2D(
        sf::Vector2f initialVelocity, 
        sf::Vector2f initialAcceleration, 
        sf::Vector2f initialPosition
    ) : Entity2D(initialPosition) {
        this->velocity = initialVelocity;
        this->acceleration = initialAcceleration;
    }

    virtual void update(float deltaTime) {
        this->velocity += acceleration * deltaTime;
        move(velocity * deltaTime);
    }

    void updateAcceleration(sf::Vector2f newAcceleration) {
        this->acceleration = newAcceleration;
    }
};

#endif // MOVABLEENTITY_H
