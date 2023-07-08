#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "piece.h"

class Game {
private:
	bool isRunning;

	int turn;
	std::vector<std::vector<int>> board;

	Piece pieces[32];
	Piece *draggedPiece;

	sf::Vector2i mousePosition;

	sf::Texture piecesTexture;
	sf::Texture boardTexture;
	sf::Sprite boardSprite;

	void loadBoard();
	void printBoard();

public:
	void loadPieces();
	void dragPiece(sf::RenderWindow& window);

	bool checkCapture(Piece& piece);
	bool checkOverlap(Piece& piece);

	void handleEvents(sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);

	void run(sf::RenderWindow& window);

	Game();
};