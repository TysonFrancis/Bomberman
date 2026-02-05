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


            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))
                bomber.setTextureRect(sf::IntRect({ 16 * count + 48, 16 }, { 16, 16 }));
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down))
                bomber.setTextureRect(sf::IntRect({ 16 * count + 48, 0 }, { 16, 16 }));
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
                bomber.setTextureRect(sf::IntRect({ 16 * count, 0 }, { 16, 16 }));
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
                bomber.setTextureRect(sf::IntRect({ 16 * count, 16 }, { 16, 16 }));
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
            {
                alive = false;
                for (int i = 0; i < 7; i++)
                {
                    bomber.setTextureRect(sf::IntRect({ 16 * i, 32 }, { 16, 16 }));
                    window.draw(bomber);
                    window.display();
                    Sleep(100);
                }
            }

            window.clear();
            window.draw(bomber);
            window.display();

            if(alive)
                count++;
            Sleep(50);
        }
    }
}