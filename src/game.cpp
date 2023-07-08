#include "game.h"
#include <SFML/Graphics/RenderWindow.hpp>

void Game::loadBoard() {
	board = {
		{ -ROOK, -KNIGHT, -BISHOP, -QUEEN, -KING, -BISHOP,  -KNIGHT, -ROOK, },
		{ -PAWN, -PAWN, 	-PAWN,   -PAWN,  -PAWN, -PAWN,    -PAWN,   -PAWN, },
		{ EMPTY, EMPTY, 	EMPTY, 	 EMPTY,  EMPTY, EMPTY,    EMPTY,   EMPTY, },
		{ EMPTY, EMPTY, 	EMPTY, 	 EMPTY,  EMPTY, EMPTY,    EMPTY,   EMPTY, },
		{ EMPTY, EMPTY, 	EMPTY, 	 EMPTY,  EMPTY, EMPTY,    EMPTY,   EMPTY, },
		{ EMPTY, EMPTY, 	EMPTY, 	 EMPTY,  EMPTY, EMPTY,    EMPTY,   EMPTY, },
		{ PAWN,  PAWN, 	PAWN,    PAWN,   PAWN,  PAWN,     PAWN,    PAWN, },
		{ ROOK,  KNIGHT,  BISHOP,  QUEEN,  KING,  BISHOP,   KNIGHT,  ROOK, }
	};

	if (!boardTexture.loadFromFile("res/board.png")) {
		std::cerr << "Couldn't load board";
		return;
	}

	boardSprite.setTexture(boardTexture);
}

void Game::loadPieces() {
	if (!piecesTexture.loadFromFile("res/pieces.png")) {
		std::cerr << "Couldn't load pieces";
		return;
	}

	int i = 0;
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			int pieceSquare = board[y][x];
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

void Game::dragPiece(sf::RenderWindow& window) {
	mousePosition = sf::Mouse::getPosition(window); 
	if (draggedPiece) {
		draggedPiece->setPosition(mousePosition.x - 32, mousePosition.y - 32);
	}
}

void Game::checkCapture(Piece& piece) {
	for (int i = 0; i < 32; i++) {
		if (pieces[i].getX() == piece.getX() && 
			pieces[i].getY() == piece.getY() &&
			pieces[i].getColor() != piece.getColor() &&
			&pieces[i] != &piece) {
				// Hiding piece
				pieces[i].setPosition(-100, -100);
		}
	}

	std::vector<int> boardCoords = piece.getBoardCoords();
	board[boardCoords[0]][boardCoords[1]] = piece.getType();
}

void Game::handleEvents(sf::RenderWindow& window) {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
			isRunning = false;
		}

		if (event.type == sf::Event::MouseButtonPressed) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				for (int i = 0; i < 32; i++) {
					if (pieces[i].isInsideBounds(mousePosition.x, mousePosition.y)) {
						if (pieces[i].getColor() != turn) {
							break;
						}
						
						draggedPiece = &pieces[i];

						std::vector<int> boardCoords = draggedPiece->getBoardCoords();

						board[boardCoords[0]][boardCoords[1]] = EMPTY;
					}
				}
			}
		}

		if (event.type == sf::Event::MouseButtonReleased) {
			if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && draggedPiece) {
				// Adjusting the position to the nearest square
				int center_x = draggedPiece->getX() + 32;
				int center_y = draggedPiece->getY() + 32;
				int x_factor = center_x / 64;
				int y_factor = center_y / 64;
				draggedPiece->setPosition(64 * x_factor, 64 * y_factor);
				checkCapture(*draggedPiece);

				//printBoard();

				system("clear");
				std::cout << draggedPiece->getCoordsInChessNotation() << std::endl;

				draggedPiece = nullptr;

				turn = !turn;
			}
		}
	}
}

void Game::draw(sf::RenderWindow& window) {
	window.clear();
		
	window.draw(boardSprite);
	for (auto piece : pieces) {
		piece.draw(window);
	}

	window.display();
}

void Game::run(sf::RenderWindow& window) {
	while (isRunning) {
		handleEvents(window);
		dragPiece(window);
		draw(window);
	}
}

Game::Game() {
	isRunning = true;
	turn = WHITE;
	draggedPiece = nullptr;
	loadBoard();
	loadPieces();
}