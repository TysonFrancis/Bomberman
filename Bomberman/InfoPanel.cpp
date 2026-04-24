#include "InfoPanel.h"
#include "Player.h"
#include "Game.h"

#include <iostream>

using std::cout, std::endl;
using namespace Constants;

InfoPanel::InfoPanel(const sf::Texture& miscTex, const sf::Texture& playTex, const Player& player) :
    lifeTex(playTex),
    giveBomb(0), giveRange(0),
    timer("time " + std::to_string(Game::getSeconds()),         // Time text
        sf::Vector2f(_infoPaneltimerX, _infoPanelLetterY),
        1, true),
    score("score ",                                             // Score text
        sf::Vector2f(_infoPanelScoreX, _infoPanelLetterY),
        -1, true)
{
    // Make gray backdrop
    backdrop.setSize(sf::Vector2f(_cols * _scaledTile, 2 * _scaledTile));
    backdrop.setFillColor(sf::Color(189, 190, 189));
    backdrop.setPosition(sf::Vector2f(0, 0));


    // Initialize powerup placements and construction
    powerUps.reserve(8);
    for (int i = 0; i < 8; i++)
    {
        auto& sprite = powerUps.emplace_back(miscTex);
        sprite.setTextureRect(sf::IntRect({ i * _tileSize, 0 }, _tile));
        sprite.setColor(sf::Color(128, 128, 128));
        sprite.setScale(sf::Vector2f(_scale, _scale));
        sprite.setPosition(sf::Vector2f
            (static_cast<float>(_infoPanelPowerupStartX + i * _scaledTile), static_cast<float>(_halfScaled)));
    }

    // Initialize life placements and construction
    lives.reserve(2);
    for (int i = 0; i < 2; i++)
    {
        auto& sprite = lives.emplace_back(lifeTex);
        sprite.setTextureRect(sf::IntRect({ 64, 0 }, _tile));
        sprite.setScale(sf::Vector2f(_scale, _scale));
        sprite.setPosition(sf::Vector2f
            (static_cast<float>(_infoPanelLivesStartX + i * _scaledTile), static_cast<float>(_halfScaled)));
    }
}

// Update game seconds and score
void InfoPanel::update()
{
    double num = Game::getSeconds();

    if (num > 1)
    {
        num /= 1000;

        std::string time = std::to_string(num);
        time.erase(0, 2);
        time.erase(3);

        timer.edit("time " + time, true);
    }

    else
        timer.edit("time 000", true);

    score.edit("score " + std::to_string(Game::getScore()), true);
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
            sprite.setPosition(sf::Vector2f(_infoPanelLivesStartX, _halfScaled));
        else
            sprite.setPosition(sf::Vector2f(_infoPanelLivesStartX + _scaledTile, _halfScaled));

        return;
    }

    lives.erase(lives.begin() + lives.size() - 1);
}

// Make powerup change colors based on bool input
void InfoPanel::updatePowerUp(PowerUp::Type type, bool turnOn)
{
    if (turnOn)
    {
        if (type == PowerUp::Type::ExtraBomb)
        {
            if (giveBomb >= _bombCountMax)      // If at max, return
                return;

            giveBomb++;                         // Increment powerup count

            if (giveBomb == 2)                          // If two powerups, make text object
                bombCount.emplace("2x", sf::Vector2f(_infoPanelBombCountX, _infoPanelPowerupLetterY), 0, true, true);
            else if (giveBomb + 1 >= _bombCountMax)     // Else if at max, edit text to say "max"
                bombCount->edit("max", true, true);
            else if (giveBomb > 2)                      // Else if more than two collected, increment text amount
                bombCount->edit(std::to_string(giveBomb) + 'x', true, true);
        }

        else if (type == PowerUp::Type::ExtraRange)
        {
            if (giveBomb >= _bombRangeMax)      // If at max, return
                return;

            giveRange++;                         // Increment powerup count

            if (giveRange == 2)                          // If two powerups, make text object
                bombRange.emplace("2x", sf::Vector2f(_infoPanelBombRangeX, _infoPanelPowerupLetterY), 0, true, true);
            else if (giveRange + 1 == _bombRangeMax)     // Else if at max, edit text to say "max"
                bombRange->edit("max", true, true);
            else if (giveRange > 2)                      // Else if more than two collected, increment text amount
                bombRange->edit(std::to_string(giveRange) + 'x', true, true);
        }

        powerUps[static_cast<int>(type)].setColor(sf::Color::White);
        return;
    }

    powerUps[static_cast<int>(type)].setColor(sf::Color(128, 128, 128));
}

void InfoPanel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(backdrop, states);          // Draw backdrop first

    for (auto& glyph : timer.sprites)       // Draw timer
        target.draw(glyph);

    for (auto& glyph : score.sprites)       // Draw score
        target.draw(glyph);

    for (auto& sprite : powerUps)           // Draw powerups
        target.draw(sprite);

    if (bombCount)                              // Draw bomb count on top of powerups
        for (auto& glyph : bombCount->sprites)
            target.draw(glyph);

    if (bombRange)                              // Draw bomb range on top of powerups
        for (auto& glyph : bombRange->sprites)
            target.draw(glyph);

    for (auto& sprite : lives)              // Draw lives
        target.draw(sprite);
}