#pragma once

#include <SFML/Graphics.hpp>
#include "piece.h"

class MoveIndicator {
private:
	float x, y;

	sf::CircleShape circle;
	sf::RectangleShape background;

public:
	void indicateCapture();

	bool isUnderPiece(Piece piece);
	
	void focus();
	void blur();

	void draw(sf::RenderWindow& window);
	
	MoveIndicator(std::vector<float> position);
};