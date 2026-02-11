#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

class Entity2D : public sf::Transformable, public sf::Drawable {
public:

    explicit Entity2D(const sf::Vector2f& initialPosition) {
        this->setPosition(initialPosition);
    }

    ~Entity2D() override = default;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

    virtual void update(float deltaTime) = 0;

    virtual sf::FloatRect getBounds() const = 0;
};

#endif // ENTITY_H
