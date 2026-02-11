#include <iostream>
#include <SFML/Graphics.hpp>

#include "constants.h"
#include "Player.h"
#include "Camera.h"
#include "Ground.h"
#include "Sky.h"
#include <memory>

enum class GameState { Playing, Dying, GameOver };

sf::Text gameOverText();

int main(int argc, char** argv) {

    srand(time(0));

    auto state = GameState::Playing;
    float deathTimer = 0.0f;

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
    // auto shape = std::make_unique<sf::RectangleShape>(sf::Vector2f(PLAYER_SIZE, PLAYER_SIZE));
    // shape->setFillColor(sf::Color::Red);
    sf::Vector2f playerInitialPosition(
        {0.0f, static_cast<float>(window.getSize().y - PLAYER_OFFSET)}
    );
    Player2D player(playerInitialPosition);
    player.switchDoubleJump();

    // CAMERA
    Camera2D camera(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT), player);

    // HANDLERS
    GroundHandler groundHandler;
    SkyHandler skyHandler;

    player.setHandler(&groundHandler);

    Sun2D sun(player);

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

        if (state == GameState::Playing) {
            player.update(dt);
            camera.update(dt);
            groundHandler.update(camera.getPosition());
            sun.update(dt);

            if (player.getPosition().y > WINDOW_HEIGHT) {
                state = GameState::Dying;
                player.velocity.x = 0.0f;
                player.collisionEnabled = false;
            }
        } else if (state == GameState::Dying) {
            deathTimer += dt;
            player.update(dt);
            camera.update(dt);
            if (deathTimer >= 2.0f) {
                state = GameState::GameOver;
            }
        }

        // BACKGROUND
        window.clear(sf::Color(100, 149, 237));

        // RENDER
        window.setView(window.getDefaultView());
        window.draw(sun);
        window.setView(camera.view);
        groundHandler.drawTo(window);
        skyHandler.drawTo(window, camera.getPosition().x, 0.1);
        window.draw(player);

        if (state == GameState::GameOver) {
            sf::Font font;
            if (!font.openFromFile("assets/PressStart2P.ttf")) {
                std::cerr << "Could not load font for arial.ttf" << std::endl;
            }
            sf::Text gameOverText(font, "       GAME OVER        \nPress any key to restart", 50);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setOrigin({gameOverText.getLocalBounds().size.x / 2.f, gameOverText.getLocalBounds().size.y / 2.f});
            gameOverText.setPosition({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f});
            window.setView(window.getDefaultView());
            window.draw(gameOverText);
        }

        window.display();
    }
    
    return 0;
}

sf::Text gameOverText() {
    sf::Font font;
    if (!font.openFromFile("assets/PressStart2P.ttf")) {
        std::cerr << "Could not load font for arial.ttf" << std::endl;
    }
    sf::Text gameOverText(font, "GAME OVER\nPress any key to restart", 50);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setOrigin({gameOverText.getLocalBounds().size.x / 2.f, gameOverText.getLocalBounds().size.y / 2.f});
    gameOverText.setPosition({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f});

    return gameOverText;
}
