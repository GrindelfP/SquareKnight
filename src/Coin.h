#ifndef COIN_H
#define COIN_H

#include "Entity.h"
#include "constants.h"

class Coin2D : public Entity2D {
public:
    sf::Texture texture;
    sf::Sprite sprite;
    bool collected = false;

    int frameCount = 29;
    int currentFrame = 0;
    float frameTime = 0.1f;
    float timer = 0.0f;
    sf::Vector2i frameSize;

    explicit Coin2D(const sf::Vector2f &position) : Entity2D(position), texture("assets/coin.png"), sprite(texture) {
        frameSize.x = static_cast<int>(texture.getSize().x);
        frameSize.y = static_cast<int>(texture.getSize().y / frameCount);

        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect({0, 0}, frameSize));

        sprite.setOrigin({static_cast<float>(frameSize.x) / 2.0f, static_cast<float>(frameSize.y) / 2.0f});
        sprite.setScale({0.5f, 0.5f});
    }

    ENTITY_METHOD void update(float deltaTime) final {
        if (!collected) {
            timer += deltaTime;
            if (timer >= frameTime) {
                timer = 0.0f;
                currentFrame = (currentFrame + 1) % frameCount;
                sprite.setTextureRect(sf::IntRect({0, currentFrame * frameSize.y}, frameSize));
            }
        }
    }

    ENTITY_METHOD void draw(sf::RenderTarget &target, sf::RenderStates states) const final {
        if (!collected) {
            states.transform *= getTransform();
            target.draw(this->sprite, states);
        }
    }

    ENTITY_METHOD sf::FloatRect getBounds() const final {
        return getTransform().transformRect(sprite.getGlobalBounds());
    }
};

#endif //COIN_H