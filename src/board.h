#pragma once

#include "piece.h"

#define KINGSIDE 1
#define QUEENSIDE -1

class Board {
private:
	std::vector<std::vector<int>> squares;
	std::vector<std::vector<std::string>> playedMoves;

	sf::Texture boardTexture;
	sf::Sprite boardSprite;

	sf::Texture piecesTexture;
	sf::Sprite piecesSprite;

	void loadPieces();

	int getSquareColor(int square);
	int getSquareColor(std::vector<int> square);

	std::vector<int> squareAbove(std::vector<int> square);
	std::vector<int> squareBelow(std::vector<int> square);
	std::vector<int> squareAtRight(std::vector<int> square);
	std::vector<int> squareAtLeft(std::vector<int> square);

	bool isInsideBoard(std::vector<int> coords);

	bool isMoveValid(Piece piece, std::vector<int> move);
	bool isKingsideCastle(Piece piece, std::vector<int> move);
	bool isQueensideCastle(Piece piece, std::vector<int> move);

	void printMoveHistory();

public:
	std::vector<Piece> pieces;

	void init();

	std::vector<int> operator[](int at) const;
	int operator[](std::vector<int> at) const;
	int operator[](std::string at) const;
	std::vector<int>& operator[](int at);
	int& operator[](std::vector<int> at);
	int& operator[](std::string at);

	bool makeMove(Piece& piece, std::vector<int> move);

	void draw(sf::RenderWindow& window);

	Board();

};