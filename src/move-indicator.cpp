#include "move-indicator.h"

// TODO
MoveIndicator::MoveIndicator(std::vector<float> position) {
	x = position[0];
	y = position[1];

	background.setSize(sf::Vector2f(64.f, 64.f));
	background.setPosition(x, y);
	circle.setRadius(10);
	circle.setOrigin(10, 10);

	blur();

	sf::Vector2f circlePos;
	circlePos.x = background.getPosition().x + background.getSize().x / 2;
	circlePos.y = background.getPosition().y + background.getSize().y / 2;
	circle.setPosition(circlePos);
}

void MoveIndicator::indicateCapture() {
	circle.setRadius(32);
	circle.setOrigin(32, 32);
}

void MoveIndicator::indicateCheck() {
	background.setFillColor(sf::Color(255, 0, 0, 200));
}

bool MoveIndicator::isUnderPiece(Piece piece) {
	sf::FloatRect bounds(x, y, background.getSize().x, background.getSize().y);

	if (piece.isInsideBounds(bounds)) {
		return true;
	}

	return false;
}

void MoveIndicator::focus() {
	background.setFillColor(sf::Color(175, 175, 175, 150));
	circle.setFillColor(sf::Color(225, 225, 225, 200));
}

void MoveIndicator::blur() {
	background.setFillColor(sf::Color(125, 125, 125, 150));
	circle.setFillColor(sf::Color(50, 50, 50, 200));
}

void MoveIndicator::draw(sf::RenderWindow& window) {
	window.draw(background);
	window.draw(circle);
}
