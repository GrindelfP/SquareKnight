#ifndef PLAYER_H
#define PLAYER_H

#include "MovableEntity.h"
#include "constants.h"
#include <SFML/Graphics.hpp>

class Player2D : public MovableEntity2D {

public:

    std::unique_ptr<sf::Shape> visual;

    static constexpr float PLAYER_X_SPEED = 30.0f;
    static constexpr float jumpVelocityY = PLAYER_BASE_JUMP_VELOCITY;

    Player2D(
        std::unique_ptr<sf::Shape> visual, 
        sf::Vector2f initialPosition
    ) : MovableEntity2D(
        sf::Vector2f{PLAYER_X_SPEED, 0.0f}, 
        sf::Vector2f{0.0f, GRAV_ACCELERATION},
        initialPosition
    ) {
        this->visual = std::move(visual);
    }

    inline void jump() {
        this->velocity.y = jumpVelocityY;
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override final {
        if (this->visual) {
            states.transform *= getTransform();
            target.draw(*this->visual, states);
        }
    }

    void update(float deltaTime) override final {
        MovableEntity2D::update(deltaTime);
        if (getPosition().y > WINDOW_HEIGHT - PLAYER_OFFSET) {
            setPosition({getPosition().x, (float)WINDOW_HEIGHT - PLAYER_OFFSET});
            this->velocity.y = 0.0f;
        }
    }
};

#endif // PLAYER_H
