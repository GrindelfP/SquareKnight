#ifndef GROUND_H
#define GROUND_H

#include "Entity.h"
#include "constants.h"
#include <random>

class Tile2D : public Entity2D {

public:

    sf::RectangleShape visual;
    sf::VertexArray dots;

    explicit Tile2D(const sf::Vector2f& tileSize, const sf::Vector2f& position) : 
        Entity2D(position), 
        visual(tileSize),
        dots(sf::PrimitiveType::Points) {
        
        sf::Color mainGreen(34, 139, 34);
        sf::Color darkGreen(0, 100, 0);
        visual.setFillColor(mainGreen);
        visual.setOutlineColor(darkGreen);
        visual.setOutlineThickness(-2.0f);

        // 1. Setup static random engine (initialized once)
        static std::random_device rd;
        static std::mt19937 gen(rd());

        // 2. Define distributions for count and coordinates
        std::uniform_int_distribution<int> countDist(100, 300);
        std::uniform_real_distribution<float> xDist(2.0f, tileSize.x - 2.0f);
        std::uniform_real_distribution<float> yDist(2.0f, tileSize.y - 2.0f);

        int dotsCount = countDist(gen);
        
        // Assuming REPEAT is a macro you have defined elsewhere
        REPEAT(dotsCount) {
            float x = xDist(gen);
            float y = yDist(gen);
            this->dots.append({sf::Vector2f(x, y), darkGreen});
        }
    }
    
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override final {
        states.transform *= getTransform();
        target.draw(this->visual, states);
        target.draw(this->dots, states);
    }

    void update(float deltaTime) override final {}
};

class GroundHandler {

public:
    std::deque<std::unique_ptr< Tile2D > > tiles;
    float lastX = -WINDOW_WIDTH;

    static constexpr float tileLenght = 100.0f;
    static constexpr float reserve = WINDOW_WIDTH;
    static constexpr float gapSize = 150.f;
    static constexpr int gaplessRowSize = 12;

    explicit GroundHandler() {
        REPEAT(gaplessRowSize) {
            spawn(false);
        }
    }

    void update(const sf::Vector2f& cameraPosition) {
        if (lastX < cameraPosition.x + reserve) {
            this->spawn();
        }
        if (tiles[0]->getPosition().x + tileLenght < cameraPosition.x - reserve) {
            tiles.pop_front();
        }
    }

    void spawn(bool addGaps = true) {
        float gap = 0.0f;
        if (addGaps && rand() % 10 > 7) gap = gapSize;
        this->lastX += (tileLenght + gap);
        this->tiles.push_back(
            std::make_unique<Tile2D>(
                sf::Vector2f(tileLenght, FLOOR_HEIGHT), 
                sf::Vector2f(lastX, WINDOW_HEIGHT - FLOOR_OFFSET)
            )
        );
    }

    void drawTo(sf::RenderTarget& target) const {
        for (const auto& tile : this->tiles) {
            target.draw(*tile);
        }
    }
};

#endif
