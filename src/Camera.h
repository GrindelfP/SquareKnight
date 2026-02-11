#ifndef CAMERA_H
#define CAMERA_H

#include "Entity.h"
#include "Follower.h"
#include <SFML/Graphics.hpp>

class Camera2D : public Entity2D, public Follower2D {

public:
    
    sf::View view;
    
    explicit Camera2D(sf::Vector2f size, const Entity2D& targetEntity) : 
        Entity2D(sf::Vector2f{size.x / 2.0f, size.y / 2.0f}), 
        Follower2D(&targetEntity) {
        this->view.setSize(size);
    }

    void update(float deltaTime) override final {
        this->follow(deltaTime);
        view.setCenter(this->getPosition());
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override final {}

    void follow(float deltaTime, float cameraSpeed = 3.5f, bool fixed = false) override final {
        sf::Vector2f targetPosition = this->target->getPosition();
        sf::Vector2f currentPosition = this->getPosition();

        float X = currentPosition.x + (targetPosition.x - currentPosition.x) * cameraSpeed * deltaTime;
        float Y = this->getPosition().y;
            
        this->setPosition({X, Y});
    }

    sf::FloatRect getBounds() const final {};
};

#endif // CAMERA_H
