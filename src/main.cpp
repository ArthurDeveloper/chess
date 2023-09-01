#include <SFML/Graphics.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <cmath>
#include "game.h"

int main() {
	int style = sf::Style::Titlebar;
    sf::RenderWindow window(sf::VideoMode(64 << 3, 64 << 3), "Chess", style);
	window.setFramerateLimit(60);

	Game game;

	game.run(window);

    return 0;
}
