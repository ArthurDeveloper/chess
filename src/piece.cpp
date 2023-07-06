#include "piece.h"
#include <SFML/Graphics/RenderWindow.hpp>

Piece::Piece() {}

Piece::Piece(int type, sf::Sprite sprite) {
	this->type = type;
	this->sprite = sprite;
}

int Piece::getX() {
	return sprite.getPosition().x;
}

int Piece::getY() {
	return sprite.getPosition().y;
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