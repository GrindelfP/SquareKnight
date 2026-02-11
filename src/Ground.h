#ifndef GROUND_H
#define GROUND_H

#include "Entity.h"
#include "constants.h"
#include <random>

#include "Coin.h"
#include "Handler.h"

class Tile2D : public Entity2D {

public:

    sf::RectangleShape visual;
    sf::VertexArray dots;
    std::unique_ptr<Coin2D> coin;

    explicit Tile2D(const sf::Vector2f& tileSize, const sf::Vector2f& position) : 
        Entity2D(position), 
        visual(tileSize),
        dots(sf::PrimitiveType::Points) {
        
        sf::Color mainGreen(34, 139, 34);
        sf::Color darkGreen(0, 100, 0);
        visual.setFillColor(mainGreen);
        visual.setOutlineColor(darkGreen);
        visual.setOutlineThickness(-2.0f);

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<int> countDist(100, 300);
        std::uniform_real_distribution<float> xDist(2.0f, tileSize.x - 2.0f);
        std::uniform_real_distribution<float> yDist(2.0f, tileSize.y - 2.0f);

        const int dotsCount = countDist(gen);
        
        REPEAT(dotsCount) {
            float x = xDist(gen);
            float y = yDist(gen);
            this->dots.append({sf::Vector2f(x, y), darkGreen});
        }

        const auto localCoinPos = sf::Vector2f(tileSize.x / 2.0f, -50.0f);
        this->coin = std::make_unique<Coin2D>(localCoinPos);
    }
    
    ENTITY_METHOD void draw(sf::RenderTarget& target, sf::RenderStates states) const final {
        states.transform *= getTransform();
        target.draw(this->visual, states);
        target.draw(this->dots, states);
        if (coin) target.draw(*coin, states);
    }

    ENTITY_METHOD void update(const float deltaTime) final {
        if (coin) {
            coin->update(deltaTime);
        }
    }

    ENTITY_METHOD sf::FloatRect getBounds() const final {
        return this->visual.getGlobalBounds();
    }

    sf::FloatRect getCoinBounds() const {
        if (!coin || coin->collected) return {};
        return getTransform().transformRect(coin->getBounds());
    }
};

class GroundHandler : public Handler {

public:
    std::deque<std::unique_ptr< Tile2D > > tiles;
    float lastX = -WINDOW_WIDTH;

    static constexpr float tileLength = 100.0f;
    static constexpr float reserve = WINDOW_WIDTH;
    static constexpr float gapSize = 150.f;
    static constexpr int gaplessRowSize = 25;

    explicit GroundHandler() {
        spawnStartingLocation();
    }

    void reset() {
        this->tiles.clear();
        this->lastX = -WINDOW_WIDTH;
        spawnStartingLocation();
    }

    void spawnStartingLocation() {
        REPEAT(gaplessRowSize) {
            spawn(false);
        }
    }

    void update(const sf::Vector2f& cameraPosition, const float deltaTime) {
        if (lastX < cameraPosition.x + reserve) {
            this->spawn();
        }
        if (tiles[0]->getPosition().x + tileLength < cameraPosition.x - reserve) {
            tiles.pop_front();
        }
        for (const auto& tile : this->tiles) {
            tile->update(deltaTime);
        }
    }

    void spawn(bool addGaps = true) {
        float gap = 0.0f;
        if (addGaps && rand() % 10 > 7) gap = gapSize;
        this->lastX += (tileLength + gap);
        this->tiles.push_back(
            std::make_unique<Tile2D>(
                sf::Vector2f(tileLength, FLOOR_HEIGHT),
                sf::Vector2f(lastX, WINDOW_HEIGHT - FLOOR_OFFSET)
            )
        );
        this->tiles[this->tiles.size() - 1]->coin->collected = !addGaps;

    }

    void drawTo(sf::RenderTarget& target) const {
        for (const auto& tile : this->tiles) {
            target.draw(*tile);
        }
    }

    [[nodiscard]] float getSurfaceHeight(const sf::FloatRect bounds) const {
        float supportL = bounds.position.x + (bounds.size.x * 0.33f);
        float supportR = bounds.position.x + (bounds.size.x * 0.66f);

        for (const auto& tile : tiles) {
            float tL = tile->getPosition().x;
            float tR = tL + tileLength;

            if (tR > supportL && tL < supportR) {
                return tile->getPosition().y;
            }
        }
        return 20000.0f;
    }
};

#endif
