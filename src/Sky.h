#ifndef SKY_H
#define SKY_H

#include "Entity.h"
#include "Follower.h"
#include "constants.h"
#include <cmath>

#include "Handler.h"

class Sun2D : public Entity2D, public Follower2D {
private:
    float accumulator = 0.0f;

public:
    std::vector<sf::CircleShape> visual;
    static constexpr int layers = 30;
    static constexpr float baseRadius = 40.0f;

    explicit Sun2D(const Entity2D &targetEntity) : Entity2D(sf::Vector2f(WINDOW_WIDTH - 550, 120)),
                                                   Follower2D(&targetEntity) {
        REPEAT(layers) {
            float currentRadius = baseRadius + (static_cast<float>(i) * 2.5f);
            sf::CircleShape layer(currentRadius);
            layer.setOrigin({currentRadius, currentRadius});
            const auto alpha = static_cast<unsigned char>(200 / (i + 1));
            layer.setFillColor(sf::Color(255, 255, 150, alpha));
            this->visual.push_back(layer);
        }
    }

    void update(float deltaTime) final {
        accumulator += deltaTime;
        float pulse = 1.0f + std::sin(accumulator * 3.0f) * 0.04f;
        this->setScale({pulse, pulse});
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const final {
        states.transform *= getTransform();
        for (auto it = visual.rbegin(); it != visual.rend(); ++it) {
            target.draw(*it, states);
        }
    }

    void follow(float deltaTime, float cameraSpeed) override { }

    sf::FloatRect getBounds() const final { return {}; }
};

class Cloud2D : public Entity2D {
public:
    std::vector<sf::CircleShape> visual;
    static constexpr int lowRadius = 30;
    static constexpr int bigRadius = 60;

    explicit Cloud2D(const sf::Vector2f &position) : Entity2D(position) {
        constexpr sf::Color white(255, 255, 255);
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution countDist(3, 7);
        std::uniform_int_distribution radiusDist(lowRadius, bigRadius);
        std::uniform_real_distribution offsetDist(-30.0f, 30.0f);

        REPEAT(countDist(gen)) {
            this->visual.emplace_back(radiusDist(gen));
            this->visual[i].setFillColor(white);
            this->visual[i].setScale(sf::Vector2f(2.0f, 1.0f));
            this->visual[i].setPosition(sf::Vector2f(
                    getPosition().x + offsetDist(gen),
                    getPosition().y + offsetDist(gen)
                )
            );
        }
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const final {
        states.transform *= getTransform();
        for (const auto &cloudPart: this->visual) {
            target.draw(cloudPart, states);
        }
    }

    void update(float deltaTime) final {
    }

    sf::FloatRect getBounds() const final { return {}; };
};

class SkyHandler : public Handler {
public:
    std::deque<std::unique_ptr<Cloud2D> > clouds;
    float lastX = -WINDOW_WIDTH;

    static constexpr float cloudRadius = 100.0f;
    static constexpr float reserve = WINDOW_WIDTH;
    static constexpr int initialCloudZone = 30;

    explicit SkyHandler() {
        REPEAT(initialCloudZone) {
            spawn();
        }
    }

    void update(const sf::Vector2f &cameraPosition) {
        if (lastX < cameraPosition.x + reserve) {
            this->spawn();
        }
        if (clouds[0]->getPosition().x + cloudRadius < cameraPosition.x - reserve) {
            clouds.pop_front();
        }
    }

    void spawn() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution gapDist(200.0f, 400.0f);
        std::uniform_real_distribution heightDist(0.0f, 200.0f);

        const float height = heightDist(gen);
        const float gap = gapDist(gen);
        this->lastX += (cloudRadius + gap);
        this->clouds.push_back(
            std::make_unique<Cloud2D>(sf::Vector2f(lastX, height))
        );
    }

    void drawTo(sf::RenderTarget &target, const float cameraCenterX, const float factor = 0.5f) const {
        float parallaxOffset = -cameraCenterX * factor;
        sf::RenderStates states;
        states.transform.translate(sf::Vector2f(parallaxOffset, 0.0f));
        for (const auto &cloud: this->clouds) {
            target.draw(*cloud, states);
        }
    }
};
#endif // SKY_H
