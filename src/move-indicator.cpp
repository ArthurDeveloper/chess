#include "move-indicator.h"

// TODO
MoveIndicator::MoveIndicator(float x, float y) {
	background.setSize(sf::Vector2f(64.f, 64.f));
	background.setPosition(x, y);
	background.setFillColor(sf::Color(125, 125, 125, 150));

	circle.setFillColor(sf::Color(50, 50, 50, 200));
	circle.setRadius(10);
	circle.setOrigin(10, 10);

	sf::Vector2f circlePos;
	circlePos.x = background.getPosition().x + background.getSize().x / 2;
	circlePos.y = background.getPosition().y + background.getSize().y / 2;
	circle.setPosition(circlePos);
}

void MoveIndicator::draw(sf::RenderWindow& window) {
	window.draw(background);
	window.draw(circle);
}
