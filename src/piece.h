#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#define KING	1
#define QUEEN	2
#define BISHOP	3
#define KNIGHT 	4
#define ROOK 	5
#define PAWN	6
#define EMPTY	0

#define BLACK 1
#define WHITE 0

class Piece {
private:
	int type;
	int color;
	sf::Sprite sprite;

	std::vector<int> lastBoardPosition;

public:
	int getX();
	int getY();
	int getColor();
	int getType();
	std::vector<int> getBoardCoords();
	std::vector<int> getLastBoardCoords();
	std::string getCoordsInChessNotation();

	void goOneMoveBack();
	void setLastBoardPosition(int x, int y);
	void setPosition(int x, int y);

	bool isInsideBounds(int x, int y);

	void draw(sf::RenderWindow& window);

	Piece();
	Piece(int type, int color, sf::Sprite sprite);

};