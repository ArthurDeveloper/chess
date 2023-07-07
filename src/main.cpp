#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <iostream>
#include <cmath>
#include <ostream>
#include <vector>
#include "piece.h"

Piece pieces[32];
Piece *draggedPiece = nullptr;

int board[8][8] = {
	-ROOK, -KNIGHT, -BISHOP, -QUEEN, -KING, -BISHOP,  -KNIGHT, -ROOK,
	-PAWN, -PAWN, 	-PAWN,   -PAWN,  -PAWN, -PAWN,    -PAWN,   -PAWN,
	EMPTY, EMPTY, 	EMPTY, 	 EMPTY,  EMPTY, EMPTY,    EMPTY,   EMPTY,
	EMPTY, EMPTY, 	EMPTY, 	 EMPTY,  EMPTY, EMPTY,    EMPTY,   EMPTY,
	EMPTY, EMPTY, 	EMPTY, 	 EMPTY,  EMPTY, EMPTY,    EMPTY,   EMPTY,
	EMPTY, EMPTY, 	EMPTY, 	 EMPTY,  EMPTY, EMPTY,    EMPTY,   EMPTY,
	PAWN,  PAWN, 	PAWN,    PAWN,   PAWN,  PAWN,     PAWN,    PAWN,
	ROOK,  KNIGHT,  BISHOP,  QUEEN,  KING,  BISHOP,   KNIGHT,  ROOK,
};

void loadPieces(sf::Texture& texture) {
	int i = 0;
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			int piece_square = board[y][x];
			if (piece_square == 0) continue;
			int color = piece_square < 0 ? BLACK : WHITE;
			piece_square = abs(piece_square);

			int size = 64;

			sf::Sprite piece_sprite;

			piece_sprite.setTexture(texture);
			piece_sprite.setTextureRect(
				sf::IntRect((piece_square - 1) * size, color * size, size, size)
			);

			piece_sprite.setPosition(x * size, y * size);

			Piece piece(piece_square, color, piece_sprite);
			pieces[i] = piece;

			i++;
		}
	}
}

void checkCapture(Piece& piece) {
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

// For debug purposes only
void printBoard() {
	system("clear");
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] == 0) {
				std::cout << "  ";
				continue;
			}
			std::cout << abs(board[i][j]) << " ";
		}
		std::cout << std::endl;
	}
}

int main() {
    sf::RenderWindow window(sf::VideoMode(64 << 3, 64 << 3), "Chess", sf::Style::Titlebar);
	window.setFramerateLimit(60);


	sf::Texture board_texture;
	if (!board_texture.loadFromFile("res/board.png")) {
		std::cerr << "Couldn't load board";
		return -1;
	}

	sf::Sprite board_sprite;
	board_sprite.setTexture(board_texture);

	sf::Texture pieces_texture;
	if (!pieces_texture.loadFromFile("res/pieces.png")) {
		std::cerr << "Couldn't load pieces";
		return -1;
	}

	loadPieces(pieces_texture);


	while (window.isOpen()) {
		sf::Vector2i mouse_position = sf::Mouse::getPosition(window); 

		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (event.type == sf::Event::MouseButtonPressed) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					for (int i = 0; i < 32; i++) {
						if (pieces[i].isInsideBounds(mouse_position.x, mouse_position.y)) {
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

					printBoard();

					draggedPiece = nullptr;
				}
			}
		}

		if (draggedPiece) {
			draggedPiece->setPosition(mouse_position.x-32, mouse_position.y-32);
		}

		window.clear();
		
		window.draw(board_sprite);
		for (auto piece : pieces) {
			piece.draw(window);
		}

		window.display();
	}

    return 0;
}
