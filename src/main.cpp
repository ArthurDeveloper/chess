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
#include "game.h"

// For debug purposes only
/*void printBoard() {
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
}*/

int main() {
    sf::RenderWindow window(sf::VideoMode(64 << 3, 64 << 3), "Chess", sf::Style::Titlebar);
	window.setFramerateLimit(60);

	Game game;

	game.run(window);

    return 0;
}
