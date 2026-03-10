//#include "Pod.h"
//
//Pod::Pod() : shape({ 0, 0 }), filled(false), tile(nullptr), x(0), y(0) {}
//
//Pod::Pod(const sf::Texture& tex, sf::RectangleShape shapes, int x, int y) :
//	shape(shapes), filled(true), tile(nullptr), x(x), y(y)
//{
//	shape.setTexture(&tex);
//	shape.setPosition(sf::Vector2f(x, y));
//}
//
//bool Pod::isObstructed()
//{
//	if (tile == nullptr)
//		return false;
//	return tile->isObstruction();
//}
//
//// Makes a new tile in this pod
//void Pod::setTile(Tile* newTile)
//{
//	delete tile; // Empties old tile before rewriting new one to prevent memeory leak
//	tile = newTile;
//	
//	if(tile != nullptr) // If has a pointer value,
//	{
//		if (tile->getType() == Tile::SOFT_WALL)		// Set to soft wall
//			setTexture(sf::IntRect({ 64, 48 }, { 16, 16 }));
//		else if (tile->getType() == Tile::BOMB)		// Set to bomb
//			setTexture(sf::IntRect({ 0, 48 }, { 16, 16 }));
//		else										// Set to empty background, these would be hard walls but 
//			setTexture(sf::IntRect({ 96, 0 }, { 16, 16 })); // lettng background texture come through instead
//	}
//	else				// Else, set inner to empty texture to let background through
//		setTexture(sf::IntRect({ 96, 0 }, { 16, 16 }));
//}
//
//Tile* Pod::getTile() const { return tile; }
//const sf::RectangleShape& Pod::getShape() const { return shape; }
//bool Pod::getFilled() const { return filled; }
//int Pod::getX() const { return x; }
//int Pod::getY() const { return y; }
//
//void Pod:: deleteTile()
//{
//	delete tile;
//	tile = nullptr;
//	setTexture(sf::IntRect({ 96, 0 }, { 16, 16 })); // Set to empty texture to let background through
//}
//
//void Pod::setColor(sf::Color color) { shape.setFillColor(color); }
//void Pod::setTexture(const sf::IntRect& rect) { shape.setTextureRect(rect); }