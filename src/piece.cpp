#include "piece.h"

Piece::Piece() {}

Piece::Piece(int type, int color, sf::Sprite sprite) {
	this->type = type;
	this->color = color;
	this->sprite = sprite;

	lastBoardPosition = getBoardCoords();
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

std::vector<int> Piece::getLastBoardCoords() {
	return lastBoardPosition;
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

std::string Piece::getCoordsInChessNotationWithCapture() {
	std::string chessNote = getCoordsInChessNotation();

	if (type == PAWN) {
		std::vector<int> lastCoords = getLastBoardCoords();

		std::string files = "abcdefgh";
		char lastFile = files[lastCoords[1]];

		return std::string({ lastFile, chessNote[0], 'x', chessNote[1], chessNote[2] });
	}

	return std::string({ chessNote[0], 'x', chessNote[1], chessNote[2] });
}

void Piece::goOneMoveBack() {
	setPosition(lastBoardPosition[1] * 64, lastBoardPosition[0] * 64);
}

void Piece::setLastBoardPosition(int rank, int file) {
	lastBoardPosition = {rank, file};
}

void Piece::setPosition(int x, int y) {
	sprite.setPosition(x, y);
}

bool Piece::isInsideBounds(int x, int y) {
	return sprite.getGlobalBounds().contains(x, y);
}

bool Piece::isInsideBounds(sf::FloatRect bounds) {
	return sprite.getGlobalBounds().intersects(bounds);
}

void Piece::draw(sf::RenderWindow& window) {
	window.draw(sprite);
}
