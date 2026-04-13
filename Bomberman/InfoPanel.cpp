#include "InfoPanel.h"
#include "Game.h"
#include <format>
#include <iostream>

using namespace Constants;

InfoPanel::InfoPanel(const sf::Texture& miscTex, const sf::Texture& playTex, const sf::Texture& titleTex) :
    lifeTex(playTex)
{
    // Make sure text texture is set
    Text::setTexture(titleTex);

    // Make gray backdrop
    backdrop.setSize(sf::Vector2f(_cols * _scaledTile, 2 * _scaledTile));
    backdrop.setFillColor(sf::Color(189, 190, 189));
    backdrop.setPosition(sf::Vector2f(0, 0));

    // Text creation
    texts.emplace_back("time " + std::to_string(Game::s_gameSeconds),         // Time text
        sf::Vector2f(_windowWidth * 0.175f, _halfScaled + _quarterScaled), 1, true);
    texts.emplace_back("score ",                                              // Score text
        sf::Vector2f(_windowWidth * 0.25f, _halfScaled + _quarterScaled), -1, true);

    // Initialize powerup placements and construction
    powerUps.reserve(8);
    for (int i = 0; i < 8; i++)
    {
        auto& sprite = powerUps.emplace_back(miscTex);
        sprite.setTextureRect(sf::IntRect({ i * _tileSize, 0 }, _tile));
        sprite.setColor(sf::Color(128, 128, 128));
        sprite.setScale(sf::Vector2f(_scale, _scale));
        sprite.setPosition(sf::Vector2f(_windowWidth * 0.475f + i * _scaledTile, _halfScaled));
    }

    // Initialize life placements and construction
    lives.reserve(2);
    for (int i = 0; i < 2; i++)
    {
        auto& sprite = lives.emplace_back(lifeTex);
        sprite.setTextureRect(sf::IntRect({ 64, 0 }, _tile));
        sprite.setScale(sf::Vector2f(_scale, _scale));
        sprite.setPosition(sf::Vector2f(_windowWidth * 0.825f + i * _scaledTile, _halfScaled));
    }
}

// Update game seconds and score
void InfoPanel::update()
{
    double num = Game::s_gameSeconds * 10 + 1;
    if (num > 1)
    {
        num /= 10000;
        std::string bingo = std::to_string(num);
        bingo.erase(0, 2);
        bingo.erase(3);
        texts[0].edit("time " + bingo, true);
    }
    else
        texts[0].edit("time 000", true);

    texts[1].edit("score " + std::to_string(Game::s_gameScore), true);
}

// Update player life display
void InfoPanel::updateLives(bool addLife)
{
    if (lives.size() < 1 && !addLife)
        return;

    if (addLife)
    {
        auto& sprite = lives.emplace_back(lifeTex);
        sprite.setTextureRect(sf::IntRect({ 64, 0 }, _tile));
        sprite.setScale(sf::Vector2f(_scale, _scale));

        if (lives.size() == 1)
            sprite.setPosition(sf::Vector2f(_windowWidth * 0.825f, _halfScaled));
        else
            sprite.setPosition(sf::Vector2f(_windowWidth * 0.825f + _scaledTile, _halfScaled));

        return;
    }

    lives.erase(lives.begin() + lives.size() - 1);
}

// Make powerup change colors based on bool input
void InfoPanel::updatePowerUp(PowerUp::Type type, bool turnOn)
{
    if (turnOn)
    {
        powerUps[static_cast<int>(type)].setColor(sf::Color::White);
        return;
    }

    powerUps[static_cast<int>(type)].setColor(sf::Color(128, 128, 128));
}

void InfoPanel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(backdrop, states);      // Draw backdrop first

    for (auto& text : texts)            // Draw text items
        for (auto& glyph : text.sprites)
            target.draw(glyph);

    for (auto& sprite : powerUps)       // Draw powerups
        target.draw(sprite);

    for (auto& sprite : lives)          // Draw lives
        target.draw(sprite);
}