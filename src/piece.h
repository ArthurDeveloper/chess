#include <SFML/Graphics.hpp>

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

public:
	int getX();
	int getY();
	int getColor();
	
	void setPosition(int x, int y);

	bool isInsideBounds(int x, int y);

	void draw(sf::RenderWindow& window);

	Piece();
	Piece(int type, int color, sf::Sprite sprite);

};