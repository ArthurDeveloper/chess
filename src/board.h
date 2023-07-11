#pragma once

#include "piece.h"

class Board {
private:
	std::vector<std::vector<int>> squares;

	sf::Texture boardTexture;
	sf::Sprite boardSprite;

	sf::Texture piecesTexture;
	sf::Sprite piecesSprite;

	void loadPieces();

public:
	std::vector<Piece> pieces;

	void init();

	std::vector<int> operator[](int at) const;
	int operator[](std::vector<int> at) const;
	std::vector<int>& operator[](int at);
	int& operator[](std::vector<int> at);

	bool makeMove(Piece& piece, std::vector<int> move);

	void draw(sf::RenderWindow& window);

	Board();

};