#pragma once

#include "piece.h"
#include "move-indicator.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <cstdlib>
#include <vector>

#define KINGSIDE 1
#define QUEENSIDE -1

class Board {
private:
	std::vector<std::vector<int>> squares;

	std::vector<std::vector<std::string>> playedMoves;
	std::vector<MoveIndicator> indicators;

	sf::Texture boardTexture;
	sf::Sprite boardSprite;

	sf::Texture piecesTexture;
	sf::Sprite piecesSprite;

	void loadPieces();

	std::vector<float> toScreenCoords(std::vector<int> boardCoords);

	int getSquareColor(int square);
	int getSquareColor(std::vector<int> square);

	std::vector<int> squareAbove(std::vector<int> square);
	std::vector<int> squareBelow(std::vector<int> square);
	std::vector<int> squareAtRight(std::vector<int> square);
	std::vector<int> squareAtLeft(std::vector<int> square);

	bool isInsideBoard(std::vector<int> coords);

	std::vector<std::vector<int>> getValidMoves(Piece piece);
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
	void indicateValidMoves(Piece piece);

	void draw(sf::RenderWindow& window);

	Board();

};