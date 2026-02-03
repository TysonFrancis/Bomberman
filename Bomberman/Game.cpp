#include "Game.h"

Game::Game()
{
    sf::Image tempImage("Textures/temp.jpg");

    window = sf::RenderWindow(sf::VideoMode({ 800, 600 }), "Game");
    window.setIcon(tempImage);
    window.setFramerateLimit(60);

    tempShape = sf::CircleShape(100.f);
}

Game::~Game() {}


void Game::run()
{
    tempShape.setFillColor(sf::Color::Green);

    while (window.isOpen())
        tick();
}

void Game::tick()
{

    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        player.tick();

        window.clear();
        window.draw(tempShape);
        window.display();
    }
}