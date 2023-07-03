#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <iostream>
#include <cmath>
#include "pieces.h"

#define BLACK 1
#define WHITE 0

sf::Sprite pieces[32];

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
			int piece = board[y][x];
			if (piece == 0) continue;
			int color = piece < 0 ? BLACK : WHITE;
			piece = abs(piece);

			int size = 64;

			pieces[i].setTexture(texture);
			pieces[i].setTextureRect(
				sf::IntRect((piece - 1) * size, color * size, size, size)
			);

			/*pieces[i].setTextureRect(
				sf::IntRect(0, 0, 56, 56)
			);*/

			pieces[i].setPosition(x * 64, y * 64);

			i++;
		}
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
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear();
		
		window.draw(board_sprite);
		for (auto piece : pieces) {
			window.draw(piece);
		}

		window.display();
	}

    return 0;
}
