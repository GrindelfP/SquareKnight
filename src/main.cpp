#include <SFML/Graphics.hpp>

#include "constants.h"
#include "Player.h"
#include <memory>

int main(int argc, char** argv) {

    // WINDOW
    sf::RenderWindow window;
    window.create(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), 
        "My first window! Hey from window!"
    );
    window.setFramerateLimit(FPS_LIMIT);

    // WORLD TIME
    sf::Clock clock;

    // PLAYER
    auto shape = std::make_unique<sf::RectangleShape>(sf::Vector2f(PLAYER_SIZE, PLAYER_SIZE));
    shape->setFillColor(sf::Color::Red);
    sf::Vector2f playerInitialPosition(
        {0.0f, static_cast<float>(window.getSize().y - PLAYER_OFFSET)}
    );
    Player2D player(std::move(shape), playerInitialPosition);

    // FLOOR
    sf::RectangleShape floor;
    floor.setSize({WINDOW_WIDTH, 20});
    floor.setFillColor(sf::Color::Green);
    floor.setPosition({0, static_cast<float>(window.getSize().y - FLOOR_OFFSET)});   

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // EVENT HANDLER
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) 
                window.close();

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Space) {
                    if (player.velocity.y == 0.0f) {
                        player.jump();
                    }
                }
            }
        }

        // UPDATE
        player.update(dt);

        // BACKGROUND
        window.clear(sf::Color(100, 149, 237));

        // RENDER
        window.draw(floor);
        window.draw(player);
        window.display();
    }
    
    return 0;
}
