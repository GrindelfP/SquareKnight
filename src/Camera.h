#ifndef CAMERA_H
#define CAMERA_H

#include "Entity.h"
#include <SFML/Graphics.hpp>

class Camera2D : public Entity2D {

public:
    
    sf::View view;
    const Entity2D* target = nullptr;

    explicit Camera2D(sf::Vector2f size, const Entity2D& targetEntity) : 
        Entity2D(sf::Vector2f{size.x / 2.0f, size.y / 2.0f}), 
        target(&targetEntity) {
        this->view.setSize(size);
    }

    void update(float deltaTime) override final {
        this->follow(deltaTime);
        view.setCenter(this->getPosition());
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override final {}

    void follow(float deltaTime) {
        float cameraSpeed = 3.5f;
        sf::Vector2f targetPosition = this->target->getPosition();
        sf::Vector2f currentPosition = this->getPosition();

        float X = currentPosition.x + (targetPosition.x - currentPosition.x) * cameraSpeed * deltaTime;
        float Y = this->getPosition().y;
        
        this->setPosition({X, Y});
    }
};

#endif // CAMERA_H
