#include "Game.h"
#include "Animations.h"

Game::Game()// : bomber("Textures/Entities.png", 16, 16, 4, 0.15f)
{
    sf::Image tempImage("Textures/temp.jpg");

    window = sf::RenderWindow(sf::VideoMode({ 256, 256 }), "Game");
    window.setIcon(tempImage);
    window.setFramerateLimit(60);

    //sf::View view = window.getDefaultView();
    //view.setSize(view.getSize() / 4.f); // zoom in 4×
    //window.setView(view);

    //bomber.setScale(16.f, 16.f);
    //bomber.setPosition(256.f, 256.f);
}

void Game::run()
{
    while (window.isOpen())
        tick();
}

void Game::tick()
{

    while (const std::optional event = window.pollEvent())
    {
        //bomber.setTextureRect(sf::IntRect({ 0, 0 }, { 16, 16 }));

        if (event->is<sf::Event::Closed>())
            window.close();

        player.tick();

        window.clear(sf::Color::Black);
        window.display();
    }
}