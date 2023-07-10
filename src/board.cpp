#include "board.h"
#include <SFML/Graphics/RenderWindow.hpp>

Board::Board() {}

void Board::init() {
	squares = {
		{ 	-ROOK, -KNIGHT, 	-BISHOP, -QUEEN, -KING, -BISHOP,  -KNIGHT, -ROOK, 		},
		{ 	-PAWN, -PAWN, 		-PAWN,   -PAWN,  -PAWN, -PAWN,    -PAWN,   -PAWN, 		},
		{ 	EMPTY, EMPTY, 		EMPTY, 	 EMPTY,  EMPTY, EMPTY,    EMPTY,   EMPTY, 		},
		{ 	EMPTY, EMPTY, 		EMPTY, 	 EMPTY,  EMPTY, EMPTY,    EMPTY,   EMPTY, 		},
		{ 	EMPTY, EMPTY, 		EMPTY, 	 EMPTY,  EMPTY, EMPTY,    EMPTY,   EMPTY, 		},
		{ 	EMPTY, EMPTY, 		EMPTY, 	 EMPTY,  EMPTY, EMPTY,    EMPTY,   EMPTY, 		},
		{ 	PAWN,  PAWN, 		PAWN,    PAWN,   PAWN,  PAWN,     PAWN,    PAWN, 		},
		{ 	ROOK,  KNIGHT,  	BISHOP,  QUEEN,  KING,  BISHOP,   KNIGHT,  ROOK, 		}
	};

	if (!boardTexture.loadFromFile("res/board.png")) {
		std::cerr << "Board could not be loaded" << std::endl;
		return;
	}

	boardSprite.setTexture(boardTexture);

	loadPieces();
}

void Board::loadPieces() {
	pieces.resize(32);

	if (!piecesTexture.loadFromFile("res/pieces.png")) {
		std::cerr << "Couldn't load pieces" << std::endl;
		return;
	}

	int i = 0;
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			int pieceSquare = squares[y][x];
			if (!pieceSquare) continue;
			int color = pieceSquare < 0 ? BLACK : WHITE;
			pieceSquare = abs(pieceSquare);

			int size = 64;

			sf::Sprite pieceSprite;

			pieceSprite.setTexture(piecesTexture);
			pieceSprite.setTextureRect(
				sf::IntRect((pieceSquare - 1) * size, color * size, size, size)
			);

			pieceSprite.setPosition(x * size, y * size);

			Piece piece(pieceSquare, color, pieceSprite);
			pieces[i] = piece;

			i++;
		}
	}
}

std::vector<int> Board::operator[](int at) const {
	return squares[at];
}

int Board::operator[](std::vector<int> at) const {
	return squares[at[0]][at[1]];
}

std::vector<int>& Board::operator[](int at) {
	return squares[at];
}

int& Board::operator[](std::vector<int> at) {
	return squares[at[0]][at[1]];
}

void Board::draw(sf::RenderWindow& window) {
	window.draw(boardSprite);

	for (int i = 0; i < pieces.size(); i++) {
		pieces[i].draw(window);
	}
}