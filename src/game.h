#include "board.h"

class Game {
private:
	bool isRunning;

	int turn;
	Board board;

	Piece *draggedPiece;

	sf::Vector2i mousePosition;

	sf::Texture piecesTexture;
	sf::Texture boardTexture;
	sf::Sprite boardSprite;

public:
	void dragPiece(sf::RenderWindow& window);

	void handleEvents(sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);

	void run(sf::RenderWindow& window);

	Game();
};