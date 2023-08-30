#pragma once

#include <SFML/Graphics.hpp>

class MoveIndicator {
private:
	float x, y;

	sf::CircleShape circle;
	sf::RectangleShape background;

public:
	void indicateCapture();

	MoveIndicator(std::vector<float> position);

	void draw(sf::RenderWindow& window);
	
};