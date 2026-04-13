#include "InfoPanel.h"
#include "Game.h"
#include <format>
using namespace Constants;

InfoPanel::InfoPanel(const sf::Texture& tex)
{
    // Make gray backdrop
    backdrop.setSize(sf::Vector2f(_cols * _scaledTile, 2 * _scaledTile));
    backdrop.setFillColor(sf::Color(189, 190, 189));
    backdrop.setPosition(sf::Vector2f(0, 0));

    // Initialize powerup placements and construction
    powerUps.reserve(8);
    for (int i = 0; i < 8; i++)
    {
        auto& sprite = powerUps.emplace_back(tex);
        sprite.setTextureRect(sf::IntRect({ i * _tileSize, 0 }, _tile));
        sprite.setColor(sf::Color(128, 128, 128));
        sprite.setScale(sf::Vector2f(_scale, _scale));
        sprite.setPosition(sf::Vector2f(_windowWidth * 0.6f + i * _scaledTile, _halfScaled));
    }

    texts.emplace_back(new Text("time " + std::to_string(Game::s_gameSeconds), sf::Vector2f(_windowWidth * 0.15f, _halfScaled), true));
    texts.emplace_back(new Text("score " + std::to_string(0), sf::Vector2f(_windowWidth * 0.35f, _halfScaled), true));
}

InfoPanel::~InfoPanel()
{
    for (Text* text : texts)      // Clean up text objects after game is finished
        delete text;
}

void InfoPanel::update()
{
    double num = Game::s_gameSeconds * 10 + 1;
    if (num > 1)
    {
        num /= 10000;
        std::string bingo = std::to_string(num);
        bingo.erase(0, 2);
        bingo.erase(3);
        texts[0]->edit(*texts[0], bingo, true);
    }
    else
        texts[0]->edit(*texts[0], "000", true);
}

// Make powerup true color when collected
void InfoPanel::updatePowerUp(PowerUp::Type type) { powerUps[static_cast<int>(type)].setColor(sf::Color::White); }

// Used to just call window.draw() in Game instead of multiple getters or infoPanel.draw()
void InfoPanel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(backdrop, states);      // Draw backdrop first

    for (auto& sprite : powerUps)       // Draw powerups on top
        target.draw(sprite);

    for (Text* text : texts)
        for (sf::Sprite* glyph : text->sprites)
            target.draw(*glyph);
}