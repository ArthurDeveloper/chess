#include "piece.h"
#include <SFML/Graphics/RenderWindow.hpp>

Piece::Piece() {}

Piece::Piece(int type, int color, sf::Sprite sprite) {
	this->type = type;
	this->color = color;
	this->sprite = sprite;
}

int Piece::getX() {
	return sprite.getPosition().x;
}

int Piece::getY() {
	return sprite.getPosition().y;
}

int Piece::getColor() {
	return color;
}

int Piece::getType() {
	return type;
}

std::vector<int> Piece::getBoardCoords() {
	std::vector<int> coords = { (int)floor((float)getY() / 64), (int)floor((float)getX() / 64) };
	return coords;
}

void Piece::setPosition(int x, int y) {
	sprite.setPosition(x, y);
}

bool Piece::isInsideBounds(int x, int y) {
	return sprite.getGlobalBounds().contains(x, y);
}

void Piece::draw(sf::RenderWindow& window) {
	window.draw(sprite);
}