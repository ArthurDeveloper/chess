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

				if (board.makeMove(*draggedPiece, { y_factor, x_factor })) {
					turn = !turn;
				}

				draggedPiece = nullptr;
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

