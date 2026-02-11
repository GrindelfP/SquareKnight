#ifndef PLAYER_H
#define PLAYER_H

#include "MovableEntity.h"
#include "constants.h"
#include <SFML/Graphics.hpp>

class Player2D : public MovableEntity2D {
public:
    std::unique_ptr<sf::Shape> visual;

    bool doubleJump = false;
    int jumps = 1;

    static constexpr float PLAYER_X_SPEED = 300.0f;
    static constexpr float jumpVelocityY = PLAYER_BASE_JUMP_VELOCITY;

    sf::Texture t1, t2;
    sf::Sprite sprite;
    float timer = 0.0f;
    int frame = 0;

    explicit Player2D(const sf::Vector2f& initialPosition) :
    MovableEntity2D(
            sf::Vector2f{PLAYER_X_SPEED, 0.0f},
            sf::Vector2f{0.0f, GRAV_ACCELERATION},
            initialPosition
        ),
        t1("assets/normal-1.png"),
        t2("assets/normal-2.png"),
        sprite(t1) {
        const float spriteSize = PLAYER_SIZE / static_cast<float>(t1.getSize().x);
        sprite.setScale(sf::Vector2f(spriteSize, spriteSize));
    }

    inline void jump() {
        if (this->jumps > 0) {
            this->velocity.y = jumpVelocityY;
            this->jumps--;
        }
    }

    ENTITY_METHOD void draw(sf::RenderTarget &target, sf::RenderStates states) const final {
        states.transform *= getTransform();
        target.draw(this->sprite, states);
    }

    ENTITY_METHOD void update(const float deltaTime) final {
        this->timer += deltaTime;
        if (this->timer >= 0.3f) {
            timer = 0.0f;
            frame = (frame + 1) % 2;
            sprite.setTexture(frame == 0 ? t1 : t2);
        }
        MovableEntity2D::update(deltaTime);
    }

    ENTITY_METHOD sf::FloatRect getBounds() const final {
        return getTransform().transformRect(sprite.getGlobalBounds());
    }

    void switchDoubleJump() {
        this->doubleJump = !(this->doubleJump);
    }

    MOVABLE_ENTITY_METHOD EVENT void onLand() override {
        this->jumps = this->doubleJump ? 2 : 1;
    }
};

#endif // PLAYER_H
