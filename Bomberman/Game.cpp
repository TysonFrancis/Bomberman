#include "Game.h"

Game::Game() : bomber(Entity::animations.getEntities())
{
    sf::Image tempImage("Textures/temp.jpg");
    window = sf::RenderWindow(sf::VideoMode({ 256, 256 }), "Game");
    window.setIcon(tempImage);
    window.setFramerateLimit(60);
    
    count = 0;
    alive = true;

    bomber.getAnimation().setScale({ 16.f, 16.f });
    bomber.getAnimation().setPosition({ 0.f, 0.f });
}

void Game::run()
{
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (count == 3)
                count = 0;

            if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Escape))
                window.close();

            window.clear();
            window.draw(bomber);
            window.display();

            if(alive)
                count++;
            Sleep(50);
        }
    }
}