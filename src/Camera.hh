#ifndef CAMERA_H
#define CAMERA_H

#include "Entity.hh"
#include "Follower.hh"
#include <SFML/Graphics.hpp>

class Camera2D : public Entity2D, public Follower2D {

public:
    
    sf::View view;
    
    explicit Camera2D(sf::Vector2f size, const Entity2D& targetEntity) : 
        Entity2D(sf::Vector2f{size.x / 2.0f, size.y / 2.0f}), 
        Follower2D(&targetEntity) {
        this->view.setSize(size);
    }

    ENTITY_METHOD void update(float deltaTime) final {
        this->follow(deltaTime, 3.5f);
        view.setCenter(this->getPosition());
    }

    ENTITY_METHOD void draw(sf::RenderTarget& target, sf::RenderStates states) const final {}

    ENTITY_METHOD sf::FloatRect getBounds() const final {
        return view.getViewport();
    }

    void follow(const float deltaTime, const float cameraSpeed) final {
        const sf::Vector2f targetPosition = this->target->getPosition();
        const sf::Vector2f currentPosition = this->getPosition();

        float X = currentPosition.x + (targetPosition.x - currentPosition.x) * cameraSpeed * deltaTime;
        float Y = this->getPosition().y;
            
        this->setPosition({X, Y});
    }
};

#endif // CAMERA_H
