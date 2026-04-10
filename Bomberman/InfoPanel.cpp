#include "InfoPanel.h"

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
}

// Make powerup true color when collected
void InfoPanel::updatePowerUp(PowerUp::Type type) { powerUps[static_cast<int>(type)].setColor(sf::Color::White); }

// Used to just call window.draw() in Game instead of multiple getters or infoPanel.draw()
void InfoPanel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(backdrop, states);      // Draw backdrop first

    for (auto& sprite : powerUps)       // Draw powerups on top
        target.draw(sprite);

                                        // Draw other info items
}