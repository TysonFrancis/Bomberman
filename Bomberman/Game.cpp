#include "Game.h"
#include "Animations.h"

Game::Game()
{
    sf::Image tempImage("Textures/temp.jpg");

    window = sf::RenderWindow(sf::VideoMode({ 750, 750 }), "Game");
    window.setIcon(tempImage);
    window.setFramerateLimit(60);

    //sf::View view = window.getDefaultView();
    //view.setSize(view.getSize() / 4.f); // zoom in 4×
    //window.setView(view);

    //bomber.setScale(16.f, 16.f);
    //bomber.setPosition(256.f, 256.f);


    //Defining pods
    for (int i = 0; i < num1; i++)
    {
        sf::RectangleShape square({ 50,50 });
        if (i % (width + x) < width)
        {
            square.setPosition(sf::Vector2f((((i - (x * (i / (x + width)))) % width) * 50),
                (((i - (x * (i / (x + width)))) / width) * 100)));
        }
        else
        {
            square.setPosition(sf::Vector2f((((i - (width * (i / (x + width))) - width) % x) * 100), 
                (i - (width * (i / (x + width))) - width) / x * 100 + 50));
        }
        Pod temp(square);

        temp.filled = (rand() % 4 == 0);
        pods.push_back(temp);

    }
    for (int i = 0; i < num2; i++)
    {
        sf::RectangleShape square({ 50,50 });
        square.setFillColor(sf::Color(255, 0, 0));
        square.setPosition(sf::Vector2f(i % (x - 1) * 100 + 50, i / (x - 1) * 100 + 50));
        Pod temp(square);
        walls.push_back(temp);
    }
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
        window.clear(sf::Color::Black);

        player.tick();

        //drawing pods
        for (int i = 0; i < num1; i++)
        {
            if (pods.at(i).filled)
            {
                window.draw(pods.at(i).shape);
            }
        }
        for (int i = 0; i < num2; i++)
        {
            window.draw(walls.at(i).shape);

        }
        
        window.display();
    }
}