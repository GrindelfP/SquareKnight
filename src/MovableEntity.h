#ifndef MOVABLEENTITY_H
#define MOVABLEENTITY_H

#include "Entity.h"
#include "Handler.h"
#include <SFML/Graphics.hpp>
#include "constants.h"
#include "Ground.h"

class MovableEntity2D : public Entity2D {
public:

    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    Handler* handler = nullptr;
    bool collisionEnabled = true;

    explicit MovableEntity2D(
        sf::Vector2f initialVelocity, 
        sf::Vector2f initialAcceleration, 
        sf::Vector2f initialPosition
    ) : Entity2D(initialPosition) {
        this->velocity = initialVelocity;
        this->acceleration = initialAcceleration;
    }

    void update(const float deltaTime) override {
        const float previousFeetY = this->getPosition().y + this->getBounds().size.y;
        this->velocity += acceleration * deltaTime;
        move(velocity * deltaTime);
        if (handler != nullptr) {
            if (this->collisionEnabled) {
                if (const auto groundHandler = dynamic_cast<GroundHandler*>(handler)) {
                    const float groundY = groundHandler->getSurfaceHeight(this->getBounds());
                    float feetY = this->getPosition().y + this->getBounds().size.y;

                    if (this->velocity.y >= 0 && feetY >= groundY && previousFeetY <= groundY + 10.0f) {
                        this->setPosition({this->getPosition().x, groundY - this->getBounds().size.y});
                        this->velocity.y = 0;
                        this->onLand();
                    }
                }
            }
        }
    }

    void setHandler(Handler* h) {
        this->handler = h;
    }

    void updateAcceleration(sf::Vector2f newAcceleration) {
        this->acceleration = newAcceleration;
    }

    EVENT virtual void onLand() {}
};

#endif // MOVABLEENTITY_H
