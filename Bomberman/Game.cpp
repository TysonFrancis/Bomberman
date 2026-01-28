#include "Game.h"

Game::Game()
{
    window = sf::RenderWindow(sf::VideoMode({ 800, 600 }), "Game");
    window.setFramerateLimit(60);
}

Game::~Game() {}

void Game::run()
{
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

}