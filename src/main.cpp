#include <SFML/Graphics.hpp>
#include <cmath>
#include "game.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(64 << 3, 64 << 3), "Chess", sf::Style::Titlebar);
	window.setFramerateLimit(60);

	Game game;

	game.run(window);

    return 0;
}
