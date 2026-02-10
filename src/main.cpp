#include <SFML/Graphics.hpp>

#include "constants.h"
#include "Player.h"
#include "Camera.h"
#include "Ground.h"
#include <memory>

int main(int argc, char** argv) {

    srand(time(0));

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
    player.switchDoubleJumb();

    // CAMERA
    Camera2D camera(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT), player);

    // FLOOR
    GroundHandler groundHandler;

    // GAME LOOP
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // EVENT HANDLER
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) 
                window.close();

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Space) {
                    player.jump();
                }
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
            }
        }

        // UPDATE
        player.update(dt);
        camera.update(dt);
        groundHandler.update(camera.getPosition());

        // BACKGROUND
        window.clear(sf::Color(100, 149, 237));

        // VIEW
        window.setView(camera.view);

        // RENDER
        groundHandler.drawTo(window);
        window.draw(player);
        window.display();
    }
    
    return 0;
}
