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

std::string Piece::getCoordsInChessNotation() {
	std::vector<int> coords = getBoardCoords();
	
	char piece;
	char file;
	char rank;

	switch (type) {
		case KING:
			piece = 'K';
			break;
		case QUEEN:
			piece = 'Q';
			break;
		case BISHOP:
			piece = 'B';
			break;
		case KNIGHT:
			piece = 'N';
			break;
		case ROOK:
			piece = 'R';
			break;
		case PAWN:
			piece = '\0';
			break;
	}


	std::string files = "abcdefgh";
	file = files[coords[1]];

	std::string ranks = "87654321";
	rank = ranks[coords[0]];

	return std::string({piece, file, rank});
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