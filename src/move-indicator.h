#pragma once

#include <SFML/Graphics.hpp>

class MoveIndicator {
private:
	float x, y;

	sf::CircleShape circle;
	sf::RectangleShape background;

public:
	MoveIndicator(float x, float y);

	void draw(sf::RenderWindow& window);

};