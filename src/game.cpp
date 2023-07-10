#include "game.h"

Game::Game() {
	isRunning = true;
	turn = WHITE;
	draggedPiece = nullptr;
	board.init();
}

void Game::dragPiece(sf::RenderWindow& window) {
	mousePosition = sf::Mouse::getPosition(window); 
	if (draggedPiece) {
		draggedPiece->setPosition(mousePosition.x - 32, mousePosition.y - 32);
	}
}

bool Game::checkCapture(Piece& piece) {
	bool hasCaptured = false;

	for (int i = 0; i < 32; i++) {
		if (board.pieces[i].getX() == piece.getX() && 
			board.pieces[i].getY() == piece.getY() &&
			board.pieces[i].getColor() != piece.getColor() && 
			&board.pieces[i] != &piece) {
				// Hiding captured piece
				board.pieces[i].setPosition(-100, -100);
				hasCaptured = true;
		}
	}

	return hasCaptured;
}

bool Game::checkOverlap(Piece& piece) {
	bool hasOverlap = false;

	for (int i = 0; i < 32; i++) {
		if (board.pieces[i].getX() == piece.getX() && 
			board.pieces[i].getY() == piece.getY() &&
			board.pieces[i].getColor() == piece.getColor() &&
			&board.pieces[i] != &piece) {
				hasOverlap = true;
				break;
		}
	}

	return hasOverlap;
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
					if (board.pieces[i].isInsideBounds(mousePosition.x, mousePosition.y)) {
						if (board.pieces[i].getColor() != turn) {
							break;
						}
						
						draggedPiece = &board.pieces[i];

						std::vector<int> boardCoords = draggedPiece->getBoardCoords();

						draggedPiece->setLastBoardPosition(boardCoords[0], boardCoords[1]);

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

				std::vector<int> boardCoords = draggedPiece->getBoardCoords();
				board[boardCoords[0]][boardCoords[1]] = draggedPiece->getType();

				checkCapture(*draggedPiece);
				if (checkOverlap(*draggedPiece)) {
					draggedPiece->goOneMoveBack();
					draggedPiece = nullptr;
					return;
				}

				std::cout << draggedPiece->getCoordsInChessNotation() << std::endl;

				draggedPiece = nullptr;

				turn = !turn;
			}
		}
	}
}

void Game::draw(sf::RenderWindow& window) {
	window.clear();
		
	board.draw(window);

	window.display();
}

void Game::run(sf::RenderWindow& window) {
	while (isRunning) {
		handleEvents(window);
		dragPiece(window);
		draw(window);
	}
}

