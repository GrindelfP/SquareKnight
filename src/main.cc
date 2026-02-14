#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "constants.hh"
#include "Player.hh"
#include "Camera.hh"
#include "Ground.hh"
#include "Sky.hh"
#include <memory>
#include <mach-o/dyld.h>
#include <unistd.h>

enum class GameState { Playing, Dying, GameOver };

int main(int argc, char **argv) {
#ifdef __APPLE__
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
        std::string fullPath(path);
        std::string dir = fullPath.substr(0, fullPath.find_last_of('/'));
        chdir(dir.c_str());
        chdir("../Resources");
    }
#endif // __APPLE__

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
    skyHandler.pin();
    player.setHandler(&groundHandler);

    Sun2D sun(player);

    // ENDGAME TEXT
    sf::Font font;
    if (!font.openFromFile("assets/PressStart2P.ttf")) {
        std::cerr << "Could not load font for arial.ttf" << std::endl;
    }
    sf::Text gameOverText(font, "       GAME OVER        \nPress any key to restart", 50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setOrigin({gameOverText.getLocalBounds().size.x / 2.f, gameOverText.getLocalBounds().size.y / 2.f});
    gameOverText.setPosition({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f});

    // COINS SCORE
    int score = 0;

    sf::Text scoreText(font, "Coins: 0", 30);
    scoreText.setPosition({50.0f, 50.0f});
    scoreText.setFillColor(sf::Color::Yellow);

    // DOUBLE JUMP
    sf::Text djText(font, "Double Jump", 30);
    djText.setPosition({WINDOW_WIDTH - 350.0f, 50.0f});

    // MUSIC
    // 1. BACKGROUND
    sf::Music bgMusic;
    if (bgMusic.openFromFile("assets/theme-2.wav")) {
        bgMusic.setLooping(true);
        bgMusic.setVolume(30.0f);
        bgMusic.play();
    }

    sf::SoundBuffer coinBuffer, milestoneBuffer;
    if (!coinBuffer.loadFromFile("assets/pickup.wav")) return -1;
    sf::Sound coinSound(coinBuffer);
    if (!milestoneBuffer.loadFromFile("assets/50coins.wav")) return -1;
    sf::Sound milestoneSound(milestoneBuffer);


    // GAME LOOP
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // EVENT HANDLER
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (state == GameState::GameOver) {
                if (event->is<sf::Event::KeyPressed>()) {
                    score = 0;
                    scoreText.setString("Score: 0");
                    state = GameState::Playing;
                    deathTimer = 0.0f;

                    player.setPosition(playerInitialPosition);
                    player.velocity = {Player2D::PLAYER_X_SPEED, 0.0f};
                    player.setHandler(&groundHandler);
                    player.collisionEnabled = true;
                    groundHandler.reset();

                    camera.setPosition({WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f});
                }
            }

            if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
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
            groundHandler.update(camera.getPosition(), dt);
            sun.update(dt);

            if (player.getPosition().y > WINDOW_HEIGHT) {
                state = GameState::Dying;
                player.velocity.x = 0.0f;
                player.collisionEnabled = false;
            }

            for (auto &tile: groundHandler.tiles) {
                if (tile->coin && !tile->coin->collected) {
                    if (player.getBounds().findIntersection(tile->getCoinBounds())) {
                        tile->coin->collected = true;
                        score++;
                        scoreText.setString("Coins: " + std::to_string(score));
                        coinSound.play();
                        if (score > 0 && score % 50 == 0) milestoneSound.play();
                    }
                }
            }

            if (player.jumps >= 1) {
                djText.setFillColor(sf::Color::Green);
            } else {
                djText.setFillColor(sf::Color::Red);
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

        window.setView(window.getDefaultView());
        window.draw(scoreText);
        window.draw(djText);

        if (state == GameState::GameOver) {
            window.draw(gameOverText);
        }

        window.display();
    }

    return 0;
}
